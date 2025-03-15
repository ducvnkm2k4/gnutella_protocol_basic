#include "messagePacket.h"

MessagePacket::MessagePacket() : MessageHeader(), payload() {}

std::vector<uint8_t> MessagePacket::getPayload() const
{
    return payload;
}

void MessagePacket::setPayload(const std::vector<uint8_t> &data)
{
    payload = data;
    setPayloadLength(data.size()); // Update payloadLength in the base class MessageHeader
}
