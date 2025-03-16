#ifndef PROTOCOL
#define PROTOCOL

#include <cstdint>
#include <array>
#include <cstring>
#include <string>

#include "getLocalIp.h"

extern int portTCP;
extern int portUDP;
extern std::array<uint8_t, 4> ipv4;
extern int ttl;
extern std::string nodeRole;

void saveConfig();
void loadConfig();
#endif