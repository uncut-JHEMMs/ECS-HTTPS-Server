#pragma once

#include <httpserver.hpp>
#include <cstdlib>
#include <fstream>
#include <utility>
#include "ServerConfiguration.h"
#include <iostream>

class Server{
private:
  unsigned int m_port = 443;
  unsigned int m_max_threads = 0;
  unsigned int m_max_connections = 0;
  unsigned int m_connection_timeout = 180;
  unsigned int m_memory_limit = 0;
  httpserver::webserver m_ws;
  
public:
  ~Server(){
    
  }
  
  Server(): m_ws(httpserver::create_webserver(443)){}
  
  Server(Server&& ser) noexcept(true) :
         m_port(std::exchange(ser.m_port, 0)),
	 m_max_threads(std::exchange(ser.m_max_threads, 0)),
	 m_max_connections(std::exchange(ser.m_max_connections, 0)),
	 m_connection_timeout(std::exchange(ser.m_memory_limit, 0)),
	 m_ws(std::move(ser.m_ws)) {
  }

  Server(const Server& ser) noexcept(true) :
         m_port(ser.m_port),
	 m_max_threads(ser.m_max_threads),
	 m_max_connections(ser.m_max_connections),
	 m_connection_timeout(ser.m_connection_timeout),
	 m_ws(ser.m_ws) {}

  Server(ServerConfiguration& config):
         m_port(config.m_port),
	 m_max_threads(config.m_max_threads),
         m_max_connections(config.m_max_connections),
	 m_connection_timeout(config.m_connection_timeout),
	 m_ws(config.m_config) {}

  Server(ServerConfiguration&& config) noexcept(true) :
         m_port(std::exchange(config.m_port, 0)),		 
	 m_max_threads(std::exchange(config.m_max_threads, 0)),
	 m_max_connections(std::exchange(config.m_max_connections, 0)),
	 m_connection_timeout(std::exchange(config.m_memory_limit, 0)),
	 m_ws(std::move(config.m_config)) {}

  Server& operator=(Server& ser) = delete;

  Server& operator=(Server&& ser) = delete;

  //true is yes false is no
  void start(bool blocking = true){
    try{
      m_ws.start(blocking);
    }catch(std::invalid_argument e){
      std::cerr << e.what() << std::endl;
    }
  }
};
