#include "server/Server.h"
#include "server/ServerConfiguration.h"
#include <utility>

int main(int argc, char** argv) {

  ServerConfiguration cnf;
  
  //If config file is provided via command line.
  if(argc > 1){
    if(!cnf.config_file(argv[1])){
      return -1;
    }
  }
  //If config file is not provided then check environment variables.
  else{
    if(!cnf.EnvironmentVarConfig()){
      
      return -1;
    }
  }
  
  Server server(std::move(cnf));
  
  server.start();

  return 0;
}
