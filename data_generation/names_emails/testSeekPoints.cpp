#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** args){

  if(argc < 2){
    std::cerr << "ERR: seekpoints or input file not provided\n";
    return -1;
  }
    
  std::ifstream seekPoints(args[1]);
  std::ifstream dataFile(args[2]);

  std::string seekPoint = "";
  std::string dataPoint = "";

  seekPoints >> seekPoint;
  seekPoints >> seekPoint;
  seekPoints >> seekPoint;
  
  int point = std::stoi(seekPoint);

  dataFile.seekg(point, dataFile.beg);
  
  std::getline(dataFile, dataPoint, '\n');

  std::cout << dataPoint << std::endl;

  seekPoints.close();
  dataFile.close();

  return 0;
}
