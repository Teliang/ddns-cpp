#include "current_ip.h"
#include "third_party/httplib.h"
#include <iostream>
#include <string>

std::string get_current_ip(std::string url) {
  std::cout << "configure server url is: " << url << std::endl;

  if (url.empty()) {
    url = std::string("http://api.ipify.org");
  }

  // HTTP
  httplib::Client cli(url);

  std::string result = "";

  if (auto res = cli.Get("/")) {
    if (res->status == httplib::StatusCode::OK_200) {
      std::cout << "http request success, body: " << res->body << std::endl;
      result = res->body;
    } else {
      std::cout << "http request fail, status code is: " << res->status
                << std::endl;
    }
  } else {
    auto err = res.error();
    std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
  }

  return result;
}
