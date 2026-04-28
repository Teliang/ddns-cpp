#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string>
#include <third_party/httplib.h>
#include <format>
#include "third_party/httplib.h"
#include "third_party/json.hpp"

std::string auth(std::string url, std::string password)
{
  // HTTP
  httplib::Client cli(url);

  std::string result = "";
  std::string json = std::format(
      R"foo({{"password":"{}"}})foo",
      password);

  if (auto res = cli.Post("/api/auth", json, "application/json"))
  {
    if (res->status == httplib::StatusCode::OK_200)
    {
      std::cout << "http request success, body: " << res->body << std::endl;

      auto json_body = nlohmann::json::parse(res->body);

      if (json_body.find("session") == json_body.end())
      {
        std::cout << "request error, body :" << std::endl;
        std::cout << res->body << std::endl;
      }

      auto json_session = json_body["session"];

      if (json_session.find("sid") == json_session.end())
      {
        std::cout << "request error, body :" << std::endl;
        std::cout << res->body << std::endl;
      }

      result = json_session["sid"];
    }
    else
    {
      std::cout << "http request fail, status code is: " << res->status
                << std::endl;
    }
  }
  else
  {
    auto err = res.error();
    std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
  }

  std::cout << "pihole auth, sid: " << result << std::endl;
  return result;
}

std::vector<std::tuple<std::string, std::string>> get_hosts_config(std::string url, std::string sid)
{
  // HTTP
  httplib::Client cli(url);

  std::vector<std::tuple<std::string, std::string>> result;

  if (auto res = cli.Get("/api/config?sid=" + sid))
  {
    if (res->status == httplib::StatusCode::OK_200)
    {
      std::cout << "http request success, body: " << res->body << std::endl;
      auto json_body = nlohmann::json::parse(res->body);
      auto json_hosts = json_body["config"]["dns"]["hosts"];

      std::cout << "json_hosts: " << json_hosts << std::endl;

      for (auto &&hosts : json_hosts)
      {
        std::string hosts_str = hosts;
        size_t pos = hosts_str.find(' ');
        result.push_back({hosts_str.substr(0, pos), hosts_str.substr(pos + 1)});
      }
    }
    else
    {
      std::cout << "http request fail, status code is: " << res->status
                << std::endl;
    }
  }
  else
  {
    auto err = res.error();
    std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
  }
  std::cout << "hosts_config: ";

  std::ranges::for_each(result, [](auto x)
                        { std::cout << std::get<0>(x) << " " << std::get<1>(x) << " "; });

  std::cout << std::endl;
  return result;
}

std::string update_hosts_config(std::string url, std::string sid, std::vector<std::tuple<std::string, std::string>> hosts)
{
  // HTTP
  httplib::Client cli(url);

  std::string json = R"({"config":{"dns":{"hosts":[)";
  std::vector<std::string> host_records;
  for (size_t i = 0; i < hosts.size(); i++)
  {
    auto &[ip, domain] = hosts[i];

    json += "\"" + ip + " " + domain + "\"";

    if (i != hosts.size() - 1)
    {
      json += ",";
    }
  }

  json += "]}}}";

  std::cout << "http request json: " << json << std::endl;
  std::string result = "";

  if (auto res = cli.Patch("/api/config?restart=true&sid=" + sid, json, "application/json"))
  {
    if (res->status == httplib::StatusCode::OK_200)
    {
      std::cout << "http request success, body: " << res->body << std::endl;
      result = res->body;
    }
    else
    {
      std::cout << "http request fail, status code is: " << res->status
                << std::endl;
    }
  }
  else
  {
    auto err = res.error();
    std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
  }

  return result;
}

void logout(std::string url, std::string sid)
{
  // HTTP
  httplib::Client cli(url);

  if (auto res = cli.Delete("/api/auth?sid=" + sid))
  {
    if (res->status == httplib::StatusCode::NoContent_204)
    {
      std::cout << "logout success" << std::endl;
    }
    else
    {
      std::cout << "http request fail, status code is: " << res->status
                << std::endl;
    }
  }
  else
  {
    auto err = res.error();
    std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
  }
}