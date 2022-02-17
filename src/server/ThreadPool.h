#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include <functional>
#include "ThreadSafeQueue.h"
#include <future>

class ThreadPool{
private:
  std::atomic_bool done;
  ThreadSafeQueue<std::function<void()>> work_queue;
  std::vector<std::thread> threads;

  void worker_thread(){
    std::function<void()> task;
    while(!done){
      if(work_queue.try_pop(task)){	
	task();
      }
      else{
	std::this_thread::yield();
      }
    }
  }

public:
  ThreadPool(): done(false){
    unsigned const thread_count = std::thread::hardware_concurrency();

    try{
      for(unsigned int i = 0; i < thread_count; ++i){
	threads.push_back(std::thread(&ThreadPool::worker_thread, this));
      }
    }catch(...){
      done = true;
      throw;
    }
  }

  ~ThreadPool(){
    done = true;

    for(std::vector<std::thread>::iterator it = threads.begin(); it != threads.end(); it++)
      it->join();
  }

  template<class FunctionType>
  void submit(FunctionType f){
    work_queue.push(std::function<void()>(f));
  }
};
