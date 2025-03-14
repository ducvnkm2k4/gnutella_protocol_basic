#include <iostream>
#include "protocol.h"
int main()
{
    std::cout << "IPv4: ";
    for (auto byte : ipv4)
    {
        std::cout << (int)byte << "."; // In từng byte của IPv4
    }
    std::cout << "\b ";
    return 0;
}