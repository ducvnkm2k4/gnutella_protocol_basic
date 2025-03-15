#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H
#include "messagePacket.h"
#include <cstring>
#include <string>
MessagePacket pingMessage();
MessagePacket pongMessage(int port, std::string ipAddress, int numFileShare, int sumKByteShare);
void queryMessage();
void queryHitMessage();

#endif
