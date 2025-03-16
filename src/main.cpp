#include "configure.h"
#include "current_ip.h"
#include "namesilo.h"

#include <chrono>
#include <iostream>
#include <thread>

int main() {
  configure conf = read_configure();

  if (conf.api_key.empty() || conf.domain.empty()) {
    std::cout << "can not read configure." << std::endl;
    return 1;
  }

  while (true) {
    std::string current_ip = get_current_ip(conf.ip_server_url);
    std::cout << "current_ip: " << current_ip << std::endl;
    std::vector<record> records = dns_list_records(conf);
    for (auto &&r : records) {
      std::cout << "DNS server configure, host: " << r.host << " ip: " << r.ip
                << std::endl;
      if (r.ip != current_ip) {
        update_record(conf, r, current_ip);
      }
    }

    // sleep
    std::this_thread::sleep_for(
        std::chrono::milliseconds(conf.execute_fix_time));
  }

  return 0;
}
