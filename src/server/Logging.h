#pragma once

#include <fstream>
#include <queue>
#include "Thread_Safe_Queue.h"
#include <ctime>
#include <chrono>
#include <thread>

class Logging{

private:
  std::ofstream m_file = std::ofstream("server_log");
  Thread_Safe_Queue<std::string> queue;
  bool done = false;

  Logging() = default;
  Logging(Logging&) = delete;
  Logging(Logging&&) = delete;
  
public:

  ~Logging(){
    if(m_file.is_open())
      m_file.close();
  }
  
  void Done(){
    done = true;
  }
  
  static Logging& createLog(){
    static Logging log;
    return log;
  }
  
  void log(const std::string& str){
    queue.push(str);
  }

  void write_thread(){
    while(!done){
      if(!queue.empty()){
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	std::string str = std::ctime(&end_time);
	m_file << str.substr(0, str.length() - 1) << ": " << queue.front() << std::endl;
	queue.pop();
      }
      else
	std::this_thread::sleep_for(std::chrono::seconds(15));
    }
    while(!queue.empty()){
      m_file << queue.front();
      queue.pop();
    }
    m_file.close();
  }
};
