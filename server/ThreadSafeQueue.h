#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>

template<class T>
class ThreadSafeQueue{
private:
  std::deque<T> queue;
  std::mutex m;
  std::condition_variable cond;
  
public:
  ThreadSafeQueue(){}

  void push(T&& val) noexcept(true){
    std::lock_guard<std::mutex> lock(m);
    queue.push_back(std::move(val));
    cond.notify_one();
  }

  void push(T& val){
    std::lock_guard<std::mutex> lock(m);
    queue.push_back(val);
    cond.notify_one();
  }

  T front(){
    std::unique_lock<std::mutex> lock(m);
    while(queue.empty())
      cond.wait(lock);
    return queue.front();
  }

  void pop(){
    std::unique_lock<std::mutex> lock(m);
    while(queue.empty())
      cond.wait(lock);
    queue.pop_front();
  }

  bool try_pop(){
    if(m.try_lock() && !queue.empty()){
      queue.pop_front();
      return true;
    }
    return false;
  }

  bool try_pop(T& val){
    std::unique_lock<std::mutex> lock(m, std::defer_lock);
    if(lock.try_lock() && !queue.empty()){
      val = queue.front();
      queue.pop_front();      
      return true;
    }
    return false;
  }
};
 
