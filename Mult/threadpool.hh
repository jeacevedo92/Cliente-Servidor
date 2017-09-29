#include "safeQueue.hh"
#include "safehash.hh"
#include <atomic>
#include <thread>

class join_threads {
  std::vector<std::thread> &threads;

public:
  explicit join_threads(std::vector<std::thread> &threads_)
      : threads(threads_) {}
  ~join_threads() {
    // std::cerr << "destructing joiner\n";
    for (unsigned long i = 0; i < threads.size(); ++i) {
      if (threads[i].joinable())
        threads[i].join();
    }
  }
};

class thread_pool {
  std::atomic_bool done;
  threadsafe_queue<std::function<void()>> work_queue;
  std::vector<std::thread> threads;
  join_threads joiner;
  threadsafe_hash<std::thread::id, int> m;

  void worker_thread() {
    while (!done) {
      std::function<void()> task;
      if (work_queue.try_pop(task)) {
        {

          auto tid = std::this_thread::get_id();
          if (m.count(tid) <= 0) {
            m.insert(tid, 1);
          } else {
            // lock_guard();
            int c = m.get(tid);
            c++;
            m.insert(tid, c);
          }
        }
        task();
      } else {
        std::this_thread::yield();
      }
    }
  }

public:
  thread_pool() : done(false), joiner(threads) {
    unsigned const thread_count = std::thread::hardware_concurrency();
    try {
      for (unsigned i = 0; i < thread_count; ++i) {
        threads.push_back(std::thread(&thread_pool::worker_thread, this));
      }
    } catch (...) {
      done = true;
      throw;
    }
  }

  ~thread_pool() { done = true; }

  template <typename FunctionType> void submit(FunctionType f) {
    work_queue.push(std::function<void()>(f));
  }
};
