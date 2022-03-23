#pragma once

#include <httpserver.hpp>
#include <cstdlib>
#include <fstream>
#include <utility>
#include "ServerConfiguration.h"
#include "resources/Hello_World_Resource.h"
#include "resources/Digest_Resource.h"
#include "Logging.h"
#include "resources/Email_Gen_resource.h"

class Server{
private:
  unsigned int m_port = 443;
  unsigned int m_max_threads = 0;
  unsigned int m_max_connections = 0;
  unsigned int m_connection_timeout = 180;
  unsigned int m_memory_limit = 0;
  httpserver::webserver m_ws;
  Logging& log = Logging::createLog();
  std::thread logging = std::thread(&Logging::write_thread, &log);
  
public:
  ~Server(){
    log.Done();
    logging.join();
  }
  
  Server(): m_ws(httpserver::create_webserver(8080)){}
  
  Server(Server&& ser) noexcept(true) :
         m_port(std::exchange(ser.m_port, 0)),
	 m_max_threads(std::exchange(ser.m_max_threads, 0)),
	 m_max_connections(std::exchange(ser.m_max_connections, 0)),
	 m_connection_timeout(std::exchange(ser.m_memory_limit, 0)),
	 m_ws(std::move(ser.m_ws)) {}

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
    log.log("Server Started");

    Hello_World_Resource r1;
    Digest_Resource r2;
    Email_Gen_resource r3;
   
    m_ws.register_resource("/hello", &r1);
    m_ws.register_resource("/auth", &r2);
    m_ws.register_resource("/users", &r3);
    
    try{
      m_ws.start(blocking);
    }catch(std::invalid_argument e){
      std::cerr << e.what() << std::endl;
    }
  }
  
  bool stop(){
    return m_ws.stop();
  }
};
