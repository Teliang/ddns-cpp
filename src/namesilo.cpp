#include "struct.h"
#include "third_party/httplib.h"
#include "third_party/json.hpp"
#include <map>
#include <vector>

std::string base_url = "https://www.namesilo.com";

std::vector<record> dns_list_records(const configure &conf) {
  std::string path = "/api/dnsListRecords?";

  std::map<std::string, std::string> paramter;

  paramter.insert({"version", "1"});
  paramter.insert({"type", "json"});
  paramter.insert({"key", conf.api_key});
  paramter.insert({"domain", conf.domain});

  for (auto &&pair : paramter) {
    path.append(pair.first);
    path.append("=");
    path.append(pair.second);
    path.append("&");
  }

  path.pop_back();

  std::cout << "dns list record url: " << base_url << path << std::endl;
  // HTTP
  httplib::Client cli(base_url);

  auto res = cli.Get(path);

  std::cout << "dns list record, response http code: " << res->status
            << std::endl;
  std::cout << res->body << std::endl;

  auto json_body = nlohmann::json::parse(res->body);

  if (json_body.find("reply") == json_body.end()) {
    std::cout << "request error, body :" << std::endl;
    std::cout << res->body << std::endl;
  }
  auto json_reply = json_body["reply"];
  if (json_reply.find("resource_record") == json_reply.end()) {
    std::cout << "request error, body :" << std::endl;
    std::cout << res->body << std::endl;
  }
  auto json_records = json_reply["resource_record"];

  std::vector<record> vec;
  for (auto &&json_record : json_records) {
    // abc.example.com -> abcに変える
    std::string host = json_record["host"];
    if (host == conf.domain) {
      host = "";
    } else {
      host = host.substr(0, host.find("."));
    }
    vec.push_back({json_record["record_id"], host, json_record["value"]});
  }
  return vec;
}

void update_record(configure &conf, record &r, std::string &ip) {
  std::string path = "/api/dnsUpdateRecord?";

  std::map<std::string, std::string> paramter = {
      {"version", "1"},        {"type", "json"}, {"key", conf.api_key},
      {"domain", conf.domain}, {"rrid", r.id},   {"rrhost", r.host},
      {"rrvalue", ip},         {"rrttl", "3600"}};

  for (auto &&pair : paramter) {
    path.append(pair.first);
    path.append("=");
    path.append(pair.second);
    path.append("&");
  }

  path.pop_back();

  // HTTP
  httplib::Client cli(base_url);

  auto res = cli.Get(path);

  std::cout << "update record response: " << std::endl;
  std::cout << res->status << std::endl;
  std::cout << res->body << std::endl;
}
