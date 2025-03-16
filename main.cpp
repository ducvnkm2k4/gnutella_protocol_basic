#include <iostream>
#include <thread>
#include "protocol.h"
#include "network.h"

int main()
{

    loadConfig();
    std::thread listentcp(listenConnectTCP,portTCP);
    listentcp.detach();
    // std::thread listenudp(listenMessageUDP,portUDP);
    // listenudp.detach();
    return 0;
}
