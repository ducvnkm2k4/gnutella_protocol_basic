#ifndef PROTOCOL
#define PROTOCOL

#include <cstdint>
#include <array>
#include "getLocalIp.h"
#include <cstring>
#include <string>
extern int port;
extern std::array<uint8_t, 4> ipv4;
extern int ttl;
extern std::string nodeRole;
#endif