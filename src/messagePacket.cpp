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

std::vector<uint8_t> MessagePacket::serialize() const
{
    std::vector<uint8_t> buffer;

    std::array<uint8_t, 16> descriptor = getdescriptorID();
    buffer.insert(buffer.end(), descriptor.begin(), descriptor.end());

    buffer.push_back(static_cast<uint8_t>(getpayloadDescriptor()));

    buffer.push_back(getTTL());

    buffer.push_back(getHops());

    return buffer;
}
