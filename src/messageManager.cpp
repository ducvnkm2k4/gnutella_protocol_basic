#include <vector>
#include <cstdint>
#include <cstring>
#include <string>
#include "messageManager.h"
#include "messagePacket.h"
#include "protocol.h"
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined(__linux__)
#include <arpa/inet.h>
#endif
// Tạo gói tin PING
MessagePacket pingMessage()
{
    MessagePacket pingPacket;
    pingPacket.setType(MessageType::PING); // Set loại gói tin là PING
    pingPacket.setTTL(ttl);                // Set TTL mặc định
    pingPacket.setHops(0);                 // Set số lần chuyển tiếp
    pingPacket.setPayloadLength(0);        // Gói tin PING không có payload
    return pingPacket;
}

// Tạo gói tin PONG
MessagePacket pongMessage(int port, std::string ipAddress, int numFileShare, int sumKByteShare)
{
    MessagePacket pongPacket;
    pongPacket.setType(MessageType::PONG); // Set loại gói tin là PONG
    pongPacket.setTTL(ttl);                // Set TTL mặc định
    pongPacket.setHops(0);                 // Set số lần chuyển tiếp

    // Chuẩn bị payload (14 bytes)
    std::vector<uint8_t> payload(14, 0); // Mặc định tất cả phần tử là 0

    // Chèn cổng (2 bytes)
    payload[0] = (port >> 8) & 0xFF;
    payload[1] = port & 0xFF;

    // Chèn địa chỉ IP (4 bytes)
    uint32_t ip = inet_addr(ipAddress.c_str()); // Chuyển IP string sang uint32_t
    std::memcpy(&payload[2], &ip, 4);

    // Do số lượng file và tổng dung lượng chia sẻ đều bằng 0, ta giữ nguyên 8 byte cuối của payload

    // Gán payload vào gói tin
    pongPacket.setPayload(payload);

    return pongPacket;
}

// Tạo gói tin QUERY
MessagePacket queryMessage(const std::vector<uint8_t> &queryData)
{
    MessagePacket queryPacket;
    queryPacket.setType(MessageType::QUERY); // Set loại gói tin là QUERY
    queryPacket.setTTL(ttl);                 // Set TTL mặc định
    queryPacket.setHops(0);                  // Set số lần chuyển tiếp
    queryPacket.setPayload(queryData);       // Gửi dữ liệu trong payload (queryData)
    return queryPacket;
}

// Tạo gói tin QUERY_HIT
MessagePacket queryHitMessage(const std::vector<uint8_t> &hitData)
{
    MessagePacket queryHitPacket;
    queryHitPacket.setType(MessageType::QUERY_HIT); // Set loại gói tin là QUERY_HIT
    queryHitPacket.setTTL(ttl);                     // Set TTL mặc định
    queryHitPacket.setHops(0);                      // Set số lần chuyển tiếp
    queryHitPacket.setPayload(hitData);             // Gửi dữ liệu trong payload (hitData)
    return queryHitPacket;
}