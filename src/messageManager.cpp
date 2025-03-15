#include <vector>
#include <cstdint>
#include "messageManager.h"
#include "messagePacket.h"
#include "protocol.h"
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
MessagePacket pongMessage(std::vector<uint8_t> payload)
{
    MessagePacket pongPacket;
    pongPacket.setType(MessageType::PONG); // Set loại gói tin là PONG
    pongPacket.setTTL(ttl);                // Set TTL mặc định
    pongPacket.setHops(0);                 // Set số lần chuyển tiếp
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

// Hàm gửi gói tin PING
void sendPing()
{
    MessagePacket ping = pingMessage(); // Tạo gói tin PING
    // Logic để gửi gói tin ping qua mạng
    // ví dụ: gửi gói tin ping qua socket hoặc giao thức mạng
}

// Hàm nhận gói tin PING
void receivePing(const MessagePacket &ping)
{
    // Logic xử lý gói tin PING khi nhận được
    // ví dụ: kiểm tra TTL, số lần chuyển tiếp, và dữ liệu trong gói tin
}

// Hàm gửi gói tin PONG
void sendPong(const MessagePacket &ping)
{
    std::vector<uint8_t> payload;
    MessagePacket pong = pongMessage(payload); // Tạo gói tin PONG
    // Logic để gửi gói tin pong qua mạng
    // ví dụ: trả lời lại gói tin PING với gói tin PONG
}

// Hàm nhận gói tin PONG
void receivePong(const MessagePacket &pong)
{
    // Logic xử lý gói tin PONG khi nhận được
    // ví dụ: kiểm tra TTL, số lần chuyển tiếp, và dữ liệu trong gói tin
}

// Hàm gửi gói tin QUERY
void sendQuery(const std::vector<uint8_t> &queryData)
{
    MessagePacket query = queryMessage(queryData); // Tạo gói tin QUERY
    // Logic để gửi gói tin query qua mạng
    // ví dụ: gửi gói tin query để tìm kiếm tài nguyên trên mạng
}

// Hàm nhận gói tin QUERY
void receiveQuery(const MessagePacket &query)
{
    // Logic xử lý gói tin QUERY khi nhận được
    // ví dụ: kiểm tra TTL, số lần chuyển tiếp, và dữ liệu trong gói tin
}

// Hàm gửi gói tin QUERY_HIT
void sendQueryHit(const std::vector<uint8_t> &hitData)
{
    MessagePacket queryHit = queryHitMessage(hitData); // Tạo gói tin QUERY_HIT
    // Logic để gửi gói tin query hit qua mạng
    // ví dụ: gửi gói tin query hit để trả về kết quả tìm kiếm
}

// Hàm nhận gói tin QUERY_HIT
void receiveQueryHit(const MessagePacket &queryHit)
{
    // Logic xử lý gói tin QUERY_HIT khi nhận được
    // ví dụ: kiểm tra TTL, số lần chuyển tiếp, và dữ liệu trong gói tin
}
