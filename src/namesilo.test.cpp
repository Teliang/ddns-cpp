#include <cassert>
#include <string>

std::string get_host(std::string host, std::string domain) {
  if (host == domain) {
    host = "";
  } else {
    host = host.substr(0, host.find(domain) - 1);
  }

  return host;
}

void case1() {
  std::string domain = "domain.com";
  std::string host = "aa.domain.com";

  assert(get_host(host, domain) == "aa");
}
void case2() {
  std::string domain = "domain.com";
  std::string host = "domain.com";

  assert(get_host(host, domain) == "");
}
void case3() {
  std::string domain = "domain.com";
  std::string host = "dd.aa.domain.com";

  assert(get_host(host, domain) == "dd.aa");
}

int main() {
  case1();
  case2();
  case3();
}
