#include "third_party/httplib.h"
#include <string>

std::string get_current_ip() {
  // HTTP
  httplib::Client cli("http://api.ipify.org");

  auto res = cli.Get("/");

  return res->body;
}
