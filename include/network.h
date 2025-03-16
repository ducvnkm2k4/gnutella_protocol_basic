#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <cstdint>
#include <cstring>
void sendMessageTCP(const std::string, int port);
void listenConnectTCP(int port);
void listenMessageUDP(int port);
void handleMessageUDP();
void sendPingMessage();
void sendPongMessage();
void sendQueryMessage();
void sendQueryHitMessage();
#endif // NETWORK_H
