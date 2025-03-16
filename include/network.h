#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <cstdint>
#include <cstring>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(__linux__)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
void sendMessageTCP(const std::string, int port);
void listenConnectTCP(int port);
void listenMessageUDP(int port);
void handleMessageUDP(const char *message, sockaddr_in &clientAddr);
void sendPingMessage(const std::string &peerIP, int port);
void sendPongMessage(sockaddr_in &clientAddr);
void sendQueryMessage();
void sendQueryHitMessage();
#endif // NETWORK_H
