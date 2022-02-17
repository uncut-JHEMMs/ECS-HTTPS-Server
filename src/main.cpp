#include <httpserver.hpp>
#include <jsoncpp/json/reader.h>
#include <fstream>
#include <string>
#include <cassert>
#include "server/Logging.h"
#include "server/ThreadPool.h"
#include "server/Server.h"
#include "server/ServerConfiguration.h"
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

  ServerConfiguration cnf;
  //If config file is provided via command line.
  if(argc > 1){
    if(!cnf.config_file(argv[1])){
      std::cout << "fail\n";
      return -1;
    }
  }
  //If config file is not provided then check environment variables.
  else{
    if(!cnf.EnvironmentVarConfig())
      return -1;
  }
  
  /*httpserver::create_webserver config(8080);
  
  config.use_ssl()
    //.https_mem_trust("cert.pem")//"pki/server/ca/certs/server.ca.cert.pem")
    .https_mem_key("key.pem")//"pki/server/ca/private/server.ca.key.pem")
    .https_mem_cert("cert.pem");//"pki/server/ca/certs/server.ca.cert.pem");

  httpserver::webserver s = config;
  std::cout << "start\n";
  s.start(true);
  */Server server(cnf);
  /*hello_world_resource hwr;
  digest_resource dg;
  
  ws.register_resource("/hello", &hwr);
  ws.register_resource("/digest", &dg);
  */
  server.start();
  return 0;
}
