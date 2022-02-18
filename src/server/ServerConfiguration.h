#pragma once

#include <iostream>
#include <json/json.h>

class Server;

class ServerConfiguration{
    
private:
  friend class Server;
  unsigned int m_port = 443;
  unsigned int m_max_threads = 0;
  unsigned int m_max_connections = 0;
  int m_connection_timeout = 180;
  unsigned int m_memory_limit = 0;
  httpserver::create_webserver m_config;
  
public:

  ServerConfiguration(){}

  //Get server configuration from environment variables
  bool EnvironmentVarConfig(){

    char* str = nullptr;
    httpserver::create_webserver config;
    
    if((str = getenv("port")) != nullptr){
      try{
	if(std::stoi(str) > 0)
	  m_port = std::stoi(str);
      } catch(const std::invalid_argument& e){
	std::cerr << "\nInvalid argument: " << e.what() << std::endl;
      }
      
      if(m_port > -1 && m_port < 65536)
	m_config.port(m_port);
      else{
	m_port = 443;
	m_config.port(443);
      }
    }
    else{
      m_port = 443;
      m_config.port(443);
    }
    
    if((str = getenv("max_threads")) != nullptr){
      try{
	if(std::stoi(str) > 0){
	  m_max_threads = std::stoi(str);
	  m_config.max_threads(m_max_threads);
	}
      } catch(const std::invalid_argument& e){
	std::cerr << "Invalid Argument: " << e.what() << std::endl;
      }
    }
    
    if((str = getenv("max_connections")) != nullptr){
      try{
	if(std::stoi(str) > 0){
	  m_max_connections = std::stoi(str);
	  m_config.max_connections(m_max_connections);
	}
      } catch(const std::invalid_argument& e){
	
        std::cerr << "Invalid Argument: " << e.what() << std::endl;
      }
    }
    
    if((str = getenv("connection_timeout")) != nullptr){
      try{
	if(std::stoi(str) > 0 && std::stoi(str) != 180){
	  m_connection_timeout = std::stoi(str);
	  m_config.connection_timeout(m_connection_timeout);
	}
      } catch(const std::invalid_argument& e){
	std::cerr << "Invalid Argument: " << e.what() << std::endl;
      }
    }
    
    if((str = getenv("memory_limit")) != nullptr){
      try{
	if(std::stoi(str) > 0){
	  m_memory_limit = std::stoi(str);
	  m_config.memory_limit(m_memory_limit); 
	}
      } catch(const std::invalid_argument& e){
	std::cerr << "Invalid Argument: " << e.what() << std::endl;
      }
    }

    m_config.use_dual_stack();
    m_config.start_method(httpserver::http::http_utils::INTERNAL_SELECT);
    
    //HTTPS configurations
    m_config.use_ssl();

    char* CA = nullptr;
    char* key = nullptr;
    char* cert = nullptr;

    if((CA = getenv("server_ca")) == nullptr ||
       (key = getenv("server_key")) == nullptr ||
       (cert = getenv("server_cert")) == nullptr){
      
      return false;
    }

    try{
      //problem. Still don't know what the difference is supposed to be between trust and certificate.
      m_config.https_mem_trust(CA)
	//rsa private key. Doesn't use aes to encrypt and thus works.
	.https_mem_key(key) 
	.https_mem_cert(cert);
    } catch(const std::invalid_argument& e){
      std::cerr << "Invalid Argument: " << e.what() << std::endl;
      return false;
    }
    return true;
  }
  
  bool config_file(const char* conf_fname){
    Json::Value read;
    std::ifstream file(conf_fname);
    //Needs error checking/exception handling.
    
    if(!file.is_open()){
      std::cerr << "Couldn't open configuration file.";
      return false;
    }
    
    file >> read;

    try{
      if((m_port = read.get("port", 443).asUInt()) != 443 && m_port < 65536)
	m_config.port(m_port);
      else{
	m_port = 443;
	m_config.port(443);
      }
    } catch(const Json::LogicError& e){
      m_config.port(443);
      std::cerr << "JSON Logic Error: " << e.what() << std::endl;      
    }

    catch(const Json::RuntimeError& e){
      std::cerr << "Json Runtime Error: " << e.what() << std::endl;
    }

    catch(...){
      std::cerr << "Json File Error\n";
    }
    
    try{
      if((m_max_threads = read.get("max_threads", 0).asUInt()) != 0){
	m_config.max_threads(m_max_threads);
      }
    }

    catch(const Json::LogicError& e){
      std::cerr << "JSON Logic Error: " << e.what() << std::endl;
    }

    catch(const Json::RuntimeError& e){                                     
      std::cerr << "Json Runtime Error: " << e.what() << std::endl;
    }

    catch(...){
      std::cerr << "Json File Error\n";
    }
    
    try{
      if((m_max_connections = read.get("max_connections", 0).asUInt()) != 0){
	m_config.max_connections(m_max_connections);
      }
    }

    catch(const Json::LogicError& e){
      std::cerr << "JSON Logic Error: " << e.what() << std::endl;
    }

    catch(const Json::RuntimeError& e){
      std::cerr << "Json Runtime Error: " << e.what() << std::endl;
    }

    catch(...){
      std::cerr << "Json File Error\n";
    }

    try{
      if((m_connection_timeout = read.get("connection_timeout", 180).asUInt()) != 180){
	m_config.connection_timeout(m_connection_timeout);
      }
    }
    catch(const Json::LogicError& e){ 
      std::cerr << "JSON Logic Error: " << e.what() << std::endl;
    }
    
    catch(const Json::RuntimeError& e){
      std::cerr << "Json Runtime Error: " << e.what() << std::endl;
    }

    catch(...){ 
      std::cerr << "Json File Error\n";
    }
      
    m_config.use_dual_stack();
    m_config.start_method(httpserver::http::http_utils::INTERNAL_SELECT);
    
    //HTTPS configurations
    m_config.use_ssl();
    std::string key = "", CA = "", cert = "";
    try{ 
      if((key = read.get("server_key", "NULL").asString()) == "NULL" ||
	 (CA = read.get("server_ca", "NULL").asString()) == "NULL" ||
	 (cert = read.get("server_cert", "NULL").asString()) == "NULL")
	return false;
    }

    catch(const Json::LogicError& e){
      std::cerr << "Json Logic Error: " << e.what() << std::endl;
      return false;
    }

    catch(const Json::RuntimeError& e){
      std::cerr << "Json Runtime Error: " << e.what() << std::endl;
      return false;
    }

    catch(...){
      std::cerr << "Json Error\n";
      return false;
    }
    //problem. Still don't know what the difference is supposed to be between trust and certificate.
    try{
      m_config.https_mem_trust(CA)
	//rsa private key. Doesn't use aes to encrypt and thus works.
	.https_mem_key(key) 
	.https_mem_cert(cert);
    }
    catch(const std::invalid_argument& e){
      std::cerr << "Invalid Argument: " << e.what() << std::endl;
      return false;
    }
    
    return true;
  }
};
