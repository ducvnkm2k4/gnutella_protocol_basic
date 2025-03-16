#include "protocol.h"
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

std::array<uint8_t, 4> ipv4 = getLocalIP();
int portTCP;
int portUDP = 6346;
int ttl;
std::string nodeRole;

std::string ipv4ToString(const std::array<uint8_t, 4> &ip)
{
    std::ostringstream oss;
    oss << static_cast<int>(ip[0]) << "."
        << static_cast<int>(ip[1]) << "."
        << static_cast<int>(ip[2]) << "."
        << static_cast<int>(ip[3]);
    return oss.str();
}

void saveConfig()
{
    std::ofstream file("app.conf");
    if (file.is_open())
    {
        file << "ipv4=" << ipv4ToString << std::endl;
        file << "portTCP=" << portTCP << std::endl;
        file << "ttl=" << ttl << std::endl;
        file << "role=" << nodeRole << std::endl;
        file.close();
    }
    else
    {
        std::cerr << "cann't open file" << std::endl;
    }
}

void loadConfig()
{
    std::ifstream file("app.conf");
    if (!file.is_open())
    {
        std::cerr << "cann't open file" << std::endl;
        return;
    }

    std::unordered_map<std::string, std::string> config;
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string key, val;
        if (std::getline(ss, key, '=') && std::getline(ss, val))
        {
            config[key] = val;
        }
    }

    if (config.find("ipv4") != config.end())
    {
        std::istringstream ipStream(config["ipv4"]);
        char dot;
        int a, b, c, d;
        if (ipStream >> a >> dot >> b >> dot >> c >> dot >> d)
        {
            ipv4 = {static_cast<uint8_t>(a), static_cast<uint8_t>(b),
                    static_cast<uint8_t>(c), static_cast<uint8_t>(d)};
        }
    }

    if (config.find("portTCP") != config.end())
        portTCP = std::stoi(config["portTCP"]);
    if (config.find("portUDP") != config.end())
        portUDP = std::stoi(config["portUDP"]);
    if (config.find("ttl") != config.end())
        ttl = std::stoi(config["ttl"]);
    if (config.find("role") != config.end())
        nodeRole = config["role"];
}
