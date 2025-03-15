#include "protocol.h"
#include <string>
#include <array>
std::array<uint8_t, 4> ipv4 = getLocalIP();
int port = 50001;
int ttl = 5;
std::string nodeRole = "leaf";
