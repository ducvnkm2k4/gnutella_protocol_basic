#ifndef MESSAGE_PACKET_H
#define MESSAGE_PACKET_H

#include <vector>
#include "messageHeader.h"

// Class definition of MessagePacket, which inherits from MessageHeader
class MessagePacket : public MessageHeader
{
private:
    std::vector<uint8_t> payload; // Payload (data)

public:
    // Default constructor
    MessagePacket();

    // Getter for payload
    std::vector<uint8_t> getPayload() const;

    // Setter for payload
    void setPayload(const std::vector<uint8_t> &data);
};

#endif
