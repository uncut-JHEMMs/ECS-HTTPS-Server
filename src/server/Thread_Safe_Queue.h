#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>

template<class T>
class Thread_Safe_Queue{
private:
  std::deque<T> queue;
  std::mutex m;
  std::condition_variable cond;
  
public:
  Thread_Safe_Queue(){}

  bool empty(){
    return queue.empty();
  }
  
  void push(T&& val) noexcept(true){
    std::lock_guard<std::mutex> lock(m);
    queue.push_back(val);
    cond.notify_one();
  }

  void push(const T& val){
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

    bool check = m.try_lock();
    if(check && !queue.empty()){
      queue.pop_front();
    }
      
    return false;
  }

  bool try_pop(T& val){
    bool check = m.try_lock();
    if(check && !queue.empty()){
      
      val = queue.front();
      queue.pop_front();
      return true;
    }
  
    return false;
  }
};
