#pragma once

#include <httpserver.hpp>
#include "../../../vendor/rapidxml-1.13/rapidxml.hpp"
#include <string.h>
#include <fstream>

class Email_Gen_resource : public httpserver::http_resource {

  const std::shared_ptr<httpserver::http_response> render_POST(const httpserver::http_request& req){

    std::string content = req.get_content();
    
    char* data = new char[content.size()];

    int i = 0;
    for(; i < content.size(); i++){
      data[i] = content[i];
    }
    
    rapidxml::xml_document<> doc;
    doc.parse<0>(data);

    std::ofstream ofile("emails.xml");
    if(!ofile.is_open()){
      //return std::shared_ptr<httpserver::http_response>(new httpserver::string_response("Failed to open new file."), httpserver::http::http_utils::http_no_content);
    }
    
    for(rapidxml::xml_node<>* node = doc.first_node(); node; node = node->next_sibling()){

      if(node->next_sibling() != nullptr && strcmp(node->name(), "first\n") && strcmp(node->next_sibling()->name(), "last\n")){
	ofile << "<email>" << node->value() << "." << node->next_sibling()->value() << "@smoothceeplusplus.com" << "</email>" << std::endl;
	node = node->next_sibling();
      }
    }
    
    ofile.close();
    return std::shared_ptr<httpserver::http_response>(new httpserver::file_response("emails.xml"));
  }
};
