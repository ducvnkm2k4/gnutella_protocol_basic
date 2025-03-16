#include <iostream>

#include "protocol.h"
#include "network.h"

int main()
{
    loadConfig();
    listenConnectTCP(portTCP);
    listenMessageUDP(portUDP);
    return 0;
}
