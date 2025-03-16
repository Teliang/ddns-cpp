#ifndef STRUCT_H
#define STRUCT_H
#include <string>

struct record {
  std::string id;
  std::string host;
  std::string ip;
};

struct configure {
  std::string ip_server_url;
  std::string domain;
  std::string api_key;
  int execute_fix_time;
};
#endif
