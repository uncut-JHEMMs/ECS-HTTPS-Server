#pragma once

#include <httpserver.hpp>
#include <fstream>
#include "../../../vendor/rapidxml-1.13/rapidxml.hpp"
#include "../charger_utils/DocumentSigning.h"
#include <memory>
#include "../charger_utils/DocumentSigning.h"

class Email_Gen_resource : public httpserver::http_resource {

private:
  
  //std::shared_ptr<DocumentSigning> signer;
  const std::shared_ptr<httpserver::http_response> render_POST(const httpserver::http_request& req){

    std::string data_str = req.get_content();

    //convert data string to cstring
    std::shared_ptr<char[]> data = std::make_shared<char[]>(data_str.size());
    for(int i = 0; i < data_str.size(); i++){
      data[i] = data_str[i];
    }

    rapidxml::xml_document<> doc;
    
    doc.parse<0>(data.get());

    std::ofstream ofile("emails.xml");
    for(rapidxml::xml_node<>* node = doc.first_node(); node; node = node->next_sibling()){
            
      if(node->next_sibling() != nullptr && strcmp(node->name(), "first\n") && strcmp(node->next_sibling()->name(), "last\n")){

        ofile << "<email>" << node->value() << "." << node->next_sibling()->value() << "@smoothceeplusplus.com" << "</email>" << std::endl;
        node = node->next_sibling();
      }	  
    }
    ofile.close();
    
    DocumentSigning signer;

    signer.sign("emails.xml");

    return std::shared_ptr<httpserver::http_response>(new httpserver::file_response("emails.xml"));
  }
};
