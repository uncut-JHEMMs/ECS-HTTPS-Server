#include "server/Server.h"
#include "server/ServerConfiguration.h"
#include <utility>
#include <iostream>
#include "server/charger_utils/DocumentSigning.h"

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

  cnf.log_access([](const std::string& str){
		   Logging::createLog().log(str);
		 });

  Server server(std::move(cnf));

  //Temporary code.
  DocumentSigning::setCert("pki/document/ca/certs/document.ca.cert.pem");
  DocumentSigning::setPrivateKey("pki/document/ca/private/document.ca.key.pem");
  server.start(true);
  //getchar();
  //server.stop();
  return 0;
}
