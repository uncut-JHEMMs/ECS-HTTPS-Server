#pragma once

#include <json/json.h>
#include <iostream>
#include <fstream>

class Certificate;

class Certificate_Configuration {

private:
  std::string common_name = "";
  std::string country = "";
  std::string state = "";
  std::string city = "";
  std::string organization = "";
  std::string organizational_unit = "";
  std::string cert_name = "";
  unsigned int days = 60 * 60 * 24 * 365;
  unsigned int bits = 2048;
  unsigned int serial = 1;
  std::string keyFname = "";
  std::string certFname = "";
  
  friend class Certificate;
  
public:
  ~Certificate_Configuration(){

  }
  
  Certificate_Configuration() = delete;
  Certificate_Configuration(const std::string& filename, const std::string& section){

    Json::Value read;
    
    std::ifstream ifile(filename.c_str());

    ifile >> read;

    try{
      if((common_name = read[section].get("common_name", "").asString()) == ""){
	std::cerr << "ERROR: Common name not defined\n";
	ifile.close();
	return;
      }
    }
    catch(const Json::LogicError& e){
      std::cerr << "JSON logic error: " << e.what() << std::endl;
      ifile.close();
      return;
    }
    catch(const Json::RuntimeError& e){
      std::cerr << "JSON Runtime Error: " << e.what() << std::endl;
      ifile.close();
      return;
    }
    
    try{
      if((country = read[section].get("country", "").asString()) == ""){

      }
    }
    catch(const Json::LogicError& e){
      std::cerr << "JSON logic error: " << e.what() << std::endl;
    }
    catch(const Json::RuntimeError& e){
      std::cerr	<< "JSON Runtime Error: " << e.what() << std::endl;
    }

    try{
      if((state = read[section].get("state", "").asString()) == ""){

      }
    }
    catch(const Json::LogicError& e){
      std::cerr << "JSON logic error: " << e.what() << std::endl;
    }
    catch(const Json::RuntimeError& e){
      std::cerr	<< "JSON Runtime Error: " << e.what() << std::endl;
    }
    
    try{
      if((city = read[section].get("city", "").asString()) == ""){

      }
    }
    catch(const Json::LogicError& e){
      std::cerr << "JSON logic error: " << e.what() << std::endl;
    }
    catch(const Json::RuntimeError& e){
      std::cerr	<< "JSON Runtime Error: " << e.what() << std::endl;
    }
    
    try{
      if((organization = read[section].get("organization", "").asString()) == ""){

      }
    }
    catch(const Json::LogicError& e){
      std::cerr << "JSON logic error: " << e.what() << std::endl;
    }
    catch(const Json::RuntimeError& e){
      std::cerr	<< "JSON Runtime Error: " << e.what() << std::endl;
    }

    try{
      if((organizational_unit = read[section].get("organizational_unit", "").asString()) == ""){

      }
    }
    catch(const Json::LogicError& e){
      std::cerr << "JSON logic error: " << e.what() << std::endl;
    }
    catch(const Json::RuntimeError& e){
      std::cerr	<< "JSON Runtime Error: " << e.what() << std::endl;
    }

    try{
      if((cert_name = read[section].get("cert_name", "").asString()) == ""){
	std::cerr << "ERROR: Certificate name not given\n";
	ifile.close();
	return;
      }
    }
    catch(const Json::LogicError& e){
      std::cerr << "JSON logic error: " << e.what() << std::endl;
      ifile.close();
      return;
    }
    catch(const Json::RuntimeError& e){
      std::cerr	<< "JSON Runtime Error: " << e.what() << std::endl;
      ifile.close();
      return;
    }
    
    try{
      if((days = read[section].get("days", days).asUInt()) == days){

      }
    }
    catch(const Json::LogicError& e){
      std::cerr << "JSON logic error: " << e.what() << std::endl;
    }
    catch(const Json::RuntimeError& e){
      std::cerr	<< "JSON Runtime Error: " << e.what() << std::endl;
    }
    
    try{
      if((bits = read[section].get("bits", bits).asUInt()) == bits){

      }
    }
    catch(const Json::LogicError& e){
      std::cerr << "JSON logic error: " << e.what() << std::endl;
    }
    catch(const Json::RuntimeError& e){
      std::cerr	<< "JSON Runtime Error: " << e.what() << std::endl;
    }

    try{
      if((serial = read[section].get("serial", 0).asUInt()) == 0){

      }
    }
    catch(const Json::LogicError& e){
      std::cerr << "JSON logic error: " << e.what() << std::endl;
    }
    catch(const Json::RuntimeError& e){
      std::cerr	<< "JSON Runtime Error: " << e.what() << std::endl;
    }

    try{
      if((keyFname = read[section].get("keyFileName", "").asString()) == ""){

	keyFname = section + ".ca.key.pem";
	std::cerr << "ERROR: Private key file name not provided.\nUsing " << keyFname << " instead.\n\n";
      }
    }
    catch(const Json::LogicError& e){
      std::cerr << "JSON logic error: " << e.what() << std::endl;
    }
    catch(const Json::RuntimeError& e){
      std::cerr << "JSON Runtime Error: " << e.what() << std::endl;
    }

    try{
      if((certFname = read[section].get("certFileName", "").asString()) == ""){
	certFname = section + ".ca.cert.pem";
	std::cerr << "ERROR: Certificate file name not provided.\nUsing " << certFname << " instead.\n\n";
      }
    }
    catch(const Json::LogicError& e){
      std::cerr << "JSON logic error: " << e.what() << std::endl;
    }
    catch(const Json::RuntimeError& e){
      std::cerr << "JSON Runtime Error: " << e.what() << std::endl;
    }

    ifile.close();
  }
};
  
