#pragma once

#define MY_OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd4"

class Digest_Resource : public httpserver::http_resource {

public:
  const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
    if(req.get_digested_user() == "") {
      return std::shared_ptr<httpserver::digest_auth_fail_response>(new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, true));
    } else {
      bool reload_nonce = false;
      if (!req.check_digest_auth("test@example.com", "mypass", 300, &reload_nonce)) {
	return std::shared_ptr<httpserver::digest_auth_fail_response>(new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, reload_nonce));
      }
    }
    return std::shared_ptr<httpserver::string_response>(new httpserver::string_response("SUCCESS", 200, "text/plain"));
  }
};
