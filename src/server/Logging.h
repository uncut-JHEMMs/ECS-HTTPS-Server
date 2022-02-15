#pragma once

#include <fstream>
#include <iostream>

class Logging{
 private:
  std::ofstream m_file;

 public:
  Logging(std::string* filename){
    try{
      m_file = std::ofstream(filename->c_str());
    }catch(...){
      std::cerr << "ERROR: Failed to open file";
    }
  }
  ~Logging(){
    m_file.close();
  }

  void log(){
    m_file << "Print to log\n";
  }
};
