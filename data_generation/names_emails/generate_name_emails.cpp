#include "vendor/name-generator/dasmig/namegen.hpp"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <iostream>

void generateName(std::unordered_map<std::wstring, char>&, std::wstring&, std::wstring&, std::wstring&, dasmig::ng::gender);

int main(int argc, char** args){

  if(argc < 2){
    std::cerr << "Input file of id's not provided.\n";
    return -1;
  }

  
  //opens the file containing the user id's created from the start.sh script.
  std::wifstream ids(args[1]);
  
  if(!ids.is_open()){
    std::cerr << "Input file does not exist\n";
    return -1;
  }
  std::ifstream serialFile("serial");
  int serialNumber = 0;
  std::string serialNumberString = "0";
  
  if(serialFile.is_open()){
    
    serialFile >> serialNumberString;
    serialFile.close();
    serialNumber = std::stoi(serialNumberString);
  }
  else{
    std::ofstream serialFileCreate("serial");
    serialFileCreate << 0;
    serialFileCreate.close();
  }
  
  //creates a file to write to in csv format
  std::wofstream csvOfile("../../src/server/resources/User_Emails_Resource/UserData" + serialNumberString + ".csv");

  //Outputs the seek points of the csv file out to a file. These points are the
  //points within the csv file that represents the beginning of a new line.
  std::ofstream seekPoints("../../src/server/resources/User_Emails_Resource/seekPoints" + serialNumberString);

  //creates a file to write to in xml format.
  std::wofstream xmlOfile("../../src/server/resources/User_Emails_Resource/UserData" + serialNumberString + ".xml");

  serialNumber++;
  std::ofstream serialFileUpdate("serial");
  serialFileUpdate << serialNumber;
  serialFileUpdate.close();
  
  char alt = 0;
  std::wstring id = L"";
  std::wstring fname;
  std::wstring lname;
  std::wstring fullname = L"";

  //holds the names generated for the output file so that we can do checks
  //for repeat names.
  std::unordered_map<std::wstring, char> repeatChecks;

  csvOfile << L"ID," << L"Firs Name," << L"Last Name," << "Email," << std::endl;
  xmlOfile << "<Document>" << std::endl;
  
  while(std::getline(ids, id)){

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
    xmlOfile << "\t\t<id>" << id << "</id>" << std::endl;
    xmlOfile << "\t\t<First>" << fname << "</First>" << std::endl;
    xmlOfile << "\t\t<Last>" << lname << "</Last>" << std::endl;
    xmlOfile << "\t\t<Email>" << fname << "." << lname << "@smoothceeplusplus.com" << std::endl;
    xmlOfile << "\t</record>" << std::endl;
  }
  xmlOfile << "</Document>";
  
  csvOfile.close();
  xmlOfile.close();
  ids.close();
  seekPoints.close();
  
  return 0;
}

/*This generates a name (male, female) and returns it using the references provided.
 * @check - This is a map containing the full names generated so that there can be checks for duplication.
 * @fname - First name.
 * @lname - Last name.
 * @fullname - Concatenated to be first and last names.
 * @morf - This is an enum determining whether to generate a mail or female name.
 */
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
