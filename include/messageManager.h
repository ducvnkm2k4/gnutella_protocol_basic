#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H
#include "messagePacket.h"
MessagePacket pingMessage();
void pongMessage();
void queryMessage();
void queryHitMessage();
void sendPing();
void receivePing();
void sendPong();
void reveivePong();
void sendQuery();
void receiveQuery();
void sendQueryHit();
void receiveQueryHit();

#endif
