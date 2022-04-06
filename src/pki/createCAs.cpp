#include <iostream>
#include <utility>
#include "Certificate_Configuration.h"
#include "Certificate.h"

int main(int argc, char** args){

  if(argc < 2){
    std::cout << "ERROR: JSON file not provided.\n";
    return -1;
  }

  //Get certificate configurations.
  std::string json_section = "RCA";
  Certificate_Configuration RCA_config(std::string(args[1]), json_section);

  json_section = "SCA";
  Certificate_Configuration SCA_config(std::string(args[1]), json_section);

  json_section = "DCA";
  Certificate_Configuration DCA_config(std::string(args[1]), json_section);

  //Create certificates from configurations.
  Certificate rca(std::move(RCA_config));
  Certificate sca(std::move(SCA_config));
  Certificate dca(std::move(DCA_config));

  //sign certificates with rca.
  rca.sign(rca);
  sca.sign(rca);
  dca.sign(rca);

  std::string certFname = "root.ca.cert.pem";
  std::string keyFname = "root.ca.key.pem";
  rca.writeToFiles();

  certFname = "server.ca.cert.pem";
  keyFname = "server.ca.key.pem";
  sca.writeToFiles();

  certFname = "document.ca.cert.pem";
  keyFname = "document.ca.key.pem";
  dca.writeToFiles();
  
  return 0;
}
