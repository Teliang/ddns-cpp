std::string auth(std::string url, std::string password);
std::vector<std::tuple<std::string, std::string>> get_hosts_config(std::string url, std::string sid);
std::string update_hosts_config(std::string url, std::string sid, std::vector<std::tuple<std::string, std::string>> hosts);
void logout(std::string url, std::string sid);