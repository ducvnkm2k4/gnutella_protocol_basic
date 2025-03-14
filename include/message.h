#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>
#include <vector>
#include <array>
#include <cstring>

namespace GnutellaPacketManager
{

} // namespace GnutellaPacketManager

namespace GnutellaPacket
{

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
        uint8_t messageID[16];  // UUID 16 byte
        MessageType type;       // Loại gói tin
        uint8_t ttl;            // Time-To-Live (TTL)
        uint8_t hops;           // Số lần đã chuyển tiếp
        uint32_t payloadLength; // Độ dài phần dữ liệu
    public:
        //  Constructor khởi tạo
        MessageHeader() : type(MessageType::PING), ttl(5), hops(0), payloadLength(0)
        {
            std::memset(messageID, 0, 16); // Khởi tạo messageID = 0
        }

        //  Getter cho messageID
        std::array<uint8_t, 16> getMessageID() const
        {
            std::array<uint8_t, 16> id;
            std::memcpy(id.data(), messageID, 16);
            return id;
        }

        //  Setter cho messageID
        void setMessageID(const std::array<uint8_t, 16> &id)
        {
            std::memcpy(messageID, id.data(), 16);
        }

        //  Getter cho type
        MessageType getType() const
        {
            return type;
        }

        //  Setter cho type
        void setType(MessageType newType)
        {
            type = newType;
        }

        //  Getter cho TTL
        uint8_t getTTL() const
        {
            return ttl;
        }

        //  Setter cho TTL
        void setTTL(uint8_t newTTL)
        {
            ttl = newTTL;
        }

        //  Getter cho hops
        uint8_t getHops() const
        {
            return hops;
        }

        //  Setter cho hops
        void setHops(uint8_t newHops)
        {
            hops = newHops;
        }

        //  Getter cho payloadLength
        uint32_t getPayloadLength() const
        {
            return payloadLength;
        }

        //  Setter cho payloadLength
        void setPayloadLength(uint32_t length)
        {
            payloadLength = length;
        }
    };
    class MessagePacket : public MessageHeader
    {
    private:
        std::vector<uint8_t> payload;

    public:
        // Constructor mặc định
        MessagePacket() : MessageHeader(), payload() {}

        // Getter cho payload
        std::vector<uint8_t> getPayload() const
        {
            return payload;
        }

        // Setter cho payload
        void setPayload(const std::vector<uint8_t> &data)
        {
            payload = data;
            setPayloadLength(data.size()); // Cập nhật độ dài payload trong MessageHeader
        }
    };

} // namespace GnutellaPacket

#endif
