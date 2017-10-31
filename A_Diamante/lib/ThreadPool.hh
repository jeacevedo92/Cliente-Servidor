#ifndef CONCURRENT_THREADPOOL_H
#define CONCURRENT_THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

/**
 *  Set to 1 to use vector instead of queue for jobs container to improve
 *  memory locality however changes job order from FIFO to LIFO.
 */
#include <queue>

/**
 *  Simple ThreadPool that creates `threadCount` threads upon its creation,
 *  and pulls from a queue to get new jobs.
 *
 *  This class requires a number of c++11 features be present in your compiler.
 */
class ThreadPool {
public:
  explicit ThreadPool(const unsigned int threadCount)
      : _jobsLeft(0), _bailout(false) {
    _threads.reserve(threadCount);
    for (unsigned int index = 0; index < threadCount; ++index) {
      _threads.push_back(std::thread([this] { this->Task(); }));
    }
  }

  /**
   *  JoinAll on deconstruction
   */
  ~ThreadPool() { JoinAll(); }

  /**
   *  Add a new job to the pool. If there are no jobs in the queue,
   *  a thread is woken up to take the job. If all threads are busy,
   *  the job is added to the end of the queue.
   */
  void AddJob(const std::function<void()> &job) {
    // scoped lock
    {
      std::lock_guard<std::mutex> lock(_queueMutex);
      _queue.push(job);
    }
    // scoped lock
    {
      std::lock_guard<std::mutex> lock(_jobsLeftMutex);
      ++_jobsLeft;
    }
    _jobAvailableVar.notify_one();
  }

  /**
   *  Join with all threads. Block until all threads have completed.
   *  The queue may be filled after this call, but the threads will
   *  be done. After invoking `ThreadPool::JoinAll`, the pool can no
   *  longer be used.
   */
  void JoinAll() {
    // scoped lock
    {
      std::lock_guard<std::mutex> lock(_queueMutex);
      if (_bailout) {
        return;
      }
      _bailout = true;
    }

    // note that we're done, and wake up any thread that's
    // waiting for a new job
    _jobAvailableVar.notify_all();

    for (std::thread &thread : _threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }

  /**
   *  Wait for the pool to empty before continuing.
   *  This does not call `std::thread::join`, it only waits until
   *  all jobs have finished executing.
   */
  void WaitAll() {
    std::unique_lock<std::mutex> lock(_jobsLeftMutex);
    if (_jobsLeft > 0) {
      _waitVar.wait(lock, [this] { return _jobsLeft == 0; });
    }
  }

  /**
   *  Get the vector of threads themselves, in order to set the
   *  affinity, or anything else you might want to do
   */
  std::vector<std::thread> &GetThreads() { return _threads; }

private:
  /**
   *  Take the next job in the queue and run it.
   *  Notify the main thread that a job has completed.
   */
  void Task() {
    while (true) {
      std::function<void()> job;

      // scoped lock
      {
        std::unique_lock<std::mutex> lock(_queueMutex);

        if (_bailout) {
          return;
        }

        // Wait for a job if we don't have any.
        _jobAvailableVar.wait(lock,
                              [this] { return !_queue.empty() || _bailout; });

        if (_bailout) {
          return;
        }

        // Get job from the queue
        job = _queue.front();
        _queue.pop();
      }

      job();

      // scoped lock
      {
        std::lock_guard<std::mutex> lock(_jobsLeftMutex);
        --_jobsLeft;
      }

      _waitVar.notify_one();
    }
  }

  std::vector<std::thread> _threads;
  std::queue<std::function<void()>> _queue;

  int _jobsLeft;
  bool _bailout;
  std::condition_variable _jobAvailableVar;
  std::condition_variable _waitVar;
  std::mutex _jobsLeftMutex;
  std::mutex _queueMutex;
};
#endif // CONCURRENT_THREADPOOL_H
