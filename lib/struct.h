#ifndef STRUCT_H
#define STRUCT_H
#include <set>
#include <string>

struct record {
  std::string id;
  std::string host;
  std::string ip;
  std::string type;
};

struct configure {
  std::string ip_server_url;
  std::string domain;
  std::string api_key;
  int execute_fix_time;
  std::set<std::string> ignore_subdomains;
};
#endif
