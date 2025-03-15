#ifndef MESSAGE_HEADER_H
#define MESSAGE_HEADER_H

#include <cstdint>
#include <array>

enum class MessageType : uint8_t
{
    PING = 0x00,
    PONG = 0x01,
    QUERY = 0x80,
    QUERY_HIT = 0x81
};

class MessageHeader
{
private:
    uint8_t descriptorID[16];      // UUID 16 byte
    MessageType payloadDescriptor; // Loại gói tin
    uint8_t ttl;                   // Time-To-Live (TTL)
    uint8_t hops;                  // Số lần đã chuyển tiếp
    uint32_t payloadLength;        // Độ dài phần dữ liệu

public:
    // Constructor
    MessageHeader();

    // Getter for messageID
    std::array<uint8_t, 16> getdescriptorID() const;

    // Setter for messageID
    void setdescriptorID(const std::array<uint8_t, 16> &id);

    // Getter for type
    MessageType getType() const;

    // Setter for type
    void setType(MessageType newType);

    // Getter for TTL
    uint8_t getTTL() const;

    // Setter for TTL
    void setTTL(uint8_t newTTL);

    // Getter for hops
    uint8_t getHops() const;

    // Setter for hops
    void setHops(uint8_t newHops);

    // Getter for payloadLength
    uint32_t getPayloadLength() const;

    // Setter for payloadLength
    void setPayloadLength(uint32_t length);
};

#endif
