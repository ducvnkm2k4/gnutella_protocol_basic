#include <iostream>
#include <thread>
#include "protocol.h"
#include "network.h"
void menu()
{
    std::cout << "----------------------MENU-------------------" << std::endl;
    std::cout << "1.send ping" << std::endl;

    std::cout << "choose?:";
}
void choose()
{
    while (true)
    {
        menu();
        int choose;
        std::cin >> choose;

        switch (choose)
        {
        case 1:
            sendPingMessage("192.168.12.2", portUDP);
            break;

        default:
            break;
        }
    }
}
int main()
{
    loadConfig();
    std::thread connecttcp(sendMessageTCP, "192.168.12.2", 50001);
    connecttcp.detach();
    std::thread udpListener(listenMessageUDP, portUDP);
    udpListener.detach();

    choose();
    return 0;
}
