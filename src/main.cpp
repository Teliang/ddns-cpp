#include "configure.h"
#include "current_ip.h"
#include "namesilo.h"
#include "pihole_api.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

void update_pihole(configure &conf, std::string &current_ip);
void update_namesilo(configure &conf, std::string &current_ip);

int main(int argc, char *argv[]) {
  std::string path = "/etc/ddns-cpp.json";

  if (argc > 1) {
    path = argv[1];
  }

  configure conf = read_configure(path);

  if (conf.api_key.empty() || conf.domain.empty()) {
    std::cout << "can not read configure." << std::endl;
    return 1;
  }

  while (true) {
    std::string current_ip = get_current_ip(conf.ip_server_url);
    std::cout << "current_ip: " << current_ip << std::endl;

    if (!current_ip.empty()) {
      update_pihole(conf, current_ip);

      update_namesilo(conf, current_ip);
    }

    // sleep
    std::this_thread::sleep_for(
        std::chrono::milliseconds(conf.execute_fix_time));
  }

  return 0;
}

void update_pihole(configure &conf, std::string &current_ip) {
  std::string url = conf.pihole_url;
  std::string password = conf.pihole_password;

  if (url.empty() || password.empty()) {
    return;
  }

  std::vector<std::string> remote_hosts = get_hosts(conf);

  std::string sid = auth(url, password);
  std::vector<std::tuple<std::string, std::string>> hosts =
      get_hosts_config(url, sid);

  bool should_update = false;

  for (const auto &remote_host : remote_hosts) {
    std::string remote_domain;
    if (remote_host == "@") {
      remote_domain = conf.domain;
    } else {
      remote_domain = remote_host + "." + conf.domain;
    }
    auto it =
        std::find_if(hosts.begin(), hosts.end(),
                     [remote_domain](std::tuple<std::string, std::string> &x) {
                       return std::get<1>(x) == remote_domain;
                     });

    if (it != hosts.end()) {
      std::string hole_ip = std::get<0>(*it);
      if (hole_ip != current_ip) {
        std::get<0>(*it) = current_ip;
        should_update = true;
      }
    } else {
      hosts.emplace_back(current_ip, remote_domain);
      should_update = true;
    }
  }

  if (should_update) {
    std::cout << "it should update pihole!" << std::endl;
    update_hosts_config(url, sid, hosts);
  }

  logout(url, sid);
}

void update_namesilo(configure &conf, std::string &current_ip) {
  std::vector<record> records = dns_list_records(conf);
  for (auto &&r : records) {
    std::cout << "DNS server configure, host: " << r.host << " ip: " << r.ip
              << std::endl;
    if ("A" != r.type) {
      std::cout << "record type is not A type!" << std::endl;
      continue;
    }

    if (conf.ignore_subdomains.find(r.host) != conf.ignore_subdomains.end()) {
      std::cout << "ignore subdomain!" << std::endl;
      continue;
    }

    if (r.ip != current_ip) {
      update_record(conf, r, current_ip);
    }
  }
}
