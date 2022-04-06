#include "vendor/name-generator/dasmig/namegen.hpp"
#include <fstream>
#include <iostream>
#include <unordered_map>

void generateName(std::unordered_map<std::wstring, char>&, std::wstring&, std::wstring&, std::wstring&, dasmig::ng::gender);

int main(int argc, char** args){

  if(argc < 3){
    std::cerr << "Input or output file not provided\n";
    return -1;
  }

  
  std::ifstream ifile(args[1]);
  if(!ifile.is_open()){
    std::cerr << "Input file does not exist\n";
    return -1;
  }

  
  std::wofstream csvOfile("../../src/server/resources/User_Emails_Resource/UserData.csv");
  std::ofstream seekPoints("../../src/server/resources/User_Emails_Resource/seekPoints");
  std::wofstream xmlOfile("../../src/server/resources/User_Emails_Resource/UserData.xml");
  
  std::string data = "";
  char alt = 0;
  int id = 0;
  std::wstring fname;
  std::wstring lname;
  std::wstring fullname = L"";
  std::unordered_map<std::wstring, char> repeatChecks;

  csvOfile << L"ID," << L"Firs Name," << L"Last Name," << "Email," << std::endl;
  xmlOfile << "<Document>" << std::endl;
  
  while(!ifile.eof()){
    std::getline(ifile, data, '\n');

    if(alt == 0){
      generateName(repeatChecks, fname, lname, fullname, dasmig::ng::gender::f);

      alt = 1;
    }
    else{
      generateName(repeatChecks, fname, lname, fullname, dasmig::ng::gender::m);

      alt = 0;
    }
    seekPoints << csvOfile.tellp() << std::endl;

    fullname = L"";
    
    csvOfile << id << "," << fname << "," << lname << "," << fname << "." << lname << "@smoothceeplusplus.com" << std::endl;

    xmlOfile << "\t<record>" << std::endl;
    xmlOfile << "\t\t<id>" << id++ << "</id>" << std::endl;
    xmlOfile << "\t\t<First>" << fname << "</First>" << std::endl;
    xmlOfile << "\t\t<Last>" << lname << "</Last>" << std::endl;
    xmlOfile << "\t\t<Email>" << fname << "." << lname << "@smoothceeplusplus.com" << std::endl;
    xmlOfile << "\t</record>" << std::endl;
  }
  xmlOfile << "</Document>";
  
  csvOfile.close();
  xmlOfile.close();
  ifile.close();
  seekPoints.close();
  
  return 0;
}

void generateName(std::unordered_map<std::wstring, char>& check, std::wstring& fname, std::wstring& lname, std::wstring& fullname, dasmig::ng::gender morf){

  fname = dasmig::ng::instance().get_name(morf, dasmig::ng::culture::american);
  lname = dasmig::ng::instance().get_surname(dasmig::ng::culture::american);
  
  fullname += fname;
  fullname += L" ";
  fullname += lname;

  if(check[fullname] != 1){
    check[fullname] = 1;
  }
  else{

    while(check[fullname] == 1){
      
      fullname = L"";
      
      fname = dasmig::ng::instance().get_name(morf, dasmig::ng::culture::american);
      lname = dasmig::ng::instance().get_surname(dasmig::ng::culture::american);
      
      fullname += fname;
      fullname += L" ";
      fullname += lname;
    }
  }
}
