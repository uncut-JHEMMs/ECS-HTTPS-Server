#pragma once

#include <httpserver.hpp>
#include <fstream>
#include "../../charger_utils/DocumentSigning.h"
#include <memory>

class Email_Gen_resource : public httpserver::http_resource {

private:
  
  const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req){

    DocumentSigning signer;
    signer.sign("server/resources/User_Emails_Resource/UserData.xml");
    
    return std::shared_ptr<httpserver::http_response>(new httpserver::file_response("signedUserData.xml"));
  }
};
