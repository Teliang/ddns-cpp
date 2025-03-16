#include "struct.h"
#include "third_party/json.hpp"
#include <asm/errno.h>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

std::string read_file(const std::string &path) {

  int fd = open(path.c_str(), O_RDONLY);

  if (fd < 0) {
    std::cout << "open file failed" << std::endl;
    return "";
  } else {

    struct stat sb;
    fstat(fd, &sb);

    std::string s;
    s.resize(sb.st_size);

    read(fd, s.data(), sb.st_size);

    close(fd);

    return s;
  }
}

configure read_configure() {
  std::string path = "/etc/ddns-cpp.json";
  auto json_conf = nlohmann::json::parse(read_file(path));
  configure conf;
  if (json_conf.find("domain") != json_conf.end()) {
    conf.domain = json_conf["domain"];
  }
  if (json_conf.find("apiKey") != json_conf.end()) {
    conf.api_key = json_conf["apiKey"];
  }
  if (json_conf.find("ipServerUrl") != json_conf.end()) {
    conf.ip_server_url = json_conf["ipServerUrl"];
  }

  if (json_conf.find("executeFixTime") != json_conf.end()) {
    conf.execute_fix_time = int(json_conf["executeFixTime"]) * 1000;
  } else {
    conf.execute_fix_time = 300 * 1000;
  }

  return conf;
}
