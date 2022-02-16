#include <httpserver.hpp>
#include "json/reader.h"
#include <fstream>
#include <cstdlib>
#include <string>
#include <cassert>
#include "ThreadPool.h"
#include <iostream>

#define MY_OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd4"

class digest_resource : public httpserver::http_resource {
 public:
     const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
         if (req.get_digested_user() == "") {
             return std::shared_ptr<httpserver::digest_auth_fail_response>(new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, true));
         } else {
             bool reload_nonce = false;
             if (!req.check_digest_auth("test@example.com", "mypass", 300, &reload_nonce)) {
                 return std::shared_ptr<httpserver::digest_auth_fail_response>(new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, reload_nonce));
             }
         }
         return std::shared_ptr<httpserver::string_response>(new httpserver::string_response("SUCCESS", 200, "text/plain"));
     }
};

class hello_world_resource : public httpserver::http_resource {
public:
  const std::shared_ptr<httpserver::http_response> render(const httpserver::http_request&) {
    return std::shared_ptr<httpserver::http_response>(new httpserver::string_response("Hello, World!"));
    }
};


int main(int argc, char** argv) {

  httpserver::create_webserver config;
  
  ThreadPool th;
  
  if(argc > 1){
    Json::Value read;
    std::ifstream file(argv[1]);
    
    if(!file.is_open()){
      std::cerr << "Couldn't open configuration file.";
      return -1;
    }

    file >> read;

    if(read.get("port", "NULL") != "NULL"){
      int port = -1;
      port = read.get("port", "NULL").asInt();
      
      assert(port > -1 && port < 65536);
      config.port(read.get("port", "NULL").asInt());
    }
    else{
      config.port(8080);
    }

    if(read.get("max_threads", "NULL") != "NULL")
      config.max_threads(read.get("max_threads", "NULL").asInt());

    if(read.get("max_connections", "NULL") != "NULL")
      config.max_connections(read.get("max_connections", "Null").asInt());

    if(read.get("connection_timeout", "NULL") != "NULL")
      config.connection_timeout(read.get("connection_timeout", "NULL").asInt());

    //Need to fill out sockaddr struct
    /*if(read.get("bind_address", "NULL") != "NULL")
      config.bind_address();*/
  }
  else{
    char* str = nullptr;

    if((str = getenv("port")) != nullptr){
      int port = std::stoi(str);
      assert(port > -1 && port < 65536);
      
      config.port(std::stoi(str));
    }
    else
      config.port(8080);
    
    if((str = getenv("max_threads")) != nullptr)
      config.max_threads(std::stoi(str));

    if((str = getenv("max_connections")) != nullptr)
      config.max_connections(std::stoi(str));

    if((str = getenv("connection_timeout")) != nullptr)
      config.connection_timeout(std::stoi(str));

    if((str = getenv("memory_limit")) != nullptr)
      config.memory_limit(std::stoi(str));
  }
  config.use_dual_stack();
  config.start_method(httpserver::http::http_utils::INTERNAL_SELECT);
  
  httpserver::webserver ws = config;
  
  hello_world_resource hwr;
  digest_resource dg;
  
  ws.register_resource("/hello", &hwr);
  ws.register_resource("/digest", &dg);
  //ws.start(true);//start the server (blocking)
  
  return 0;
}
