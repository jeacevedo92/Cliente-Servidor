#include <condition_variable>
#include <mutex>
#include <unordered_map>

using namespace std;

template <typename K, typename V> class threadsafe_hash {
private:
  mutable mutex mut;
  unordered_map<K, V> data;
  condition_variable data_cond;

public:
  threadsafe_hash() {}

  bool empty() const {
    lock_guard<mutex> lk(mut);
    return data.empty();
  }

  int size() {
    lock_guard<mutex> lk(mut);
    return data.size();
  }

  void insert(K key, V value) {
    lock_guard<mutex> lk(mut);
    data[key] = move(value);
    data_cond.notify_one();
  }

  void wait_and_get(K key, V &ans) {
    unique_lock<mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data.empty(); }); // ?
    ans = move(data[key]);
  }

  shared_ptr<V> wait_and_get(K key) {
    unique_lock<mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data.empty(); }); // ?
    shared_ptr<V> res(make_shared<V>(move(data[key])));
    return res;
  }

  bool try_get(K key, V &ans) {
    lock_guard<mutex> lk(mut);
    if (data.empty())
      return false;
    ans = move(data[key]);
    return true;
  }

  V &get(K key) {
    lock_guard<mutex> lk(mut);
    return data[key];
  }

  shared_ptr<V> try_get(K key) {
    lock_guard<mutex> lk(mut);
    if (data.empty())
      return shared_ptr<V>();
    shared_ptr<V> res(make_shared<V>(move(data[key])));
    return res;
  }

  int count(K key) {
    lock_guard<mutex> lk(mut);
    return data.count(key);
  }
};
