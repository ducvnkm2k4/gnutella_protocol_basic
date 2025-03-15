#include "messageHeader.h"
#include <cstring> // cho std::memset và std::memcpy

MessageHeader::MessageHeader() : payloadDescriptor(MessageType::PING), ttl(5), hops(0), payloadLength(0)
{
    std::memset(descriptorID, 0, 16); // Khởi tạo descriptorID = 0
}

std::array<uint8_t, 16> MessageHeader::getdescriptorID() const
{
    std::array<uint8_t, 16> id;
    std::memcpy(id.data(), descriptorID, 16);
    return id;
}

void MessageHeader::setdescriptorID(const std::array<uint8_t, 16> &id)
{
    std::memcpy(descriptorID, id.data(), 16);
}

MessageType MessageHeader::getType() const
{
    return payloadDescriptor;
}

void MessageHeader::setType(MessageType newType)
{
    payloadDescriptor = newType;
}

uint8_t MessageHeader::getTTL() const
{
    return ttl;
}

void MessageHeader::setTTL(uint8_t newTTL)
{
    ttl = newTTL;
}

uint8_t MessageHeader::getHops() const
{
    return hops;
}

void MessageHeader::setHops(uint8_t newHops)
{
    hops = newHops;
}

uint32_t MessageHeader::getPayloadLength() const
{
    return payloadLength;
}

void MessageHeader::setPayloadLength(uint32_t length)
{
    payloadLength = length;
}
