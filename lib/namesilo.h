#include "struct.h"
#include <string>
#include <vector>

std::vector<record> dns_list_records(const configure &conf);
void update_record(configure &conf, record &r, std::string &ip);
