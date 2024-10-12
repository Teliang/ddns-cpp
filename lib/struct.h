#pragma once
#include <string>

struct record {
  std::string id;
  std::string host;
  std::string ip;
};

struct configure {
  std::string domain;
  std::string api_key;
  int execute_fix_time;
};
