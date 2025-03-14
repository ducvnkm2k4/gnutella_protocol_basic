#include "get_local_ip.h"

#include <iostream> // Thư viện nhập xuất
#include <array>    // Sử dụng std::array để lưu địa chỉ IP
#include <cstring>  // Hỗ trợ thao tác với bộ nhớ (memcpy)
#include <cstdint>  // Định nghĩa kiểu uint8_t (8-bit không dấu)

#ifdef _WIN32                      // Nếu đang biên dịch trên Windows
#include <winsock2.h>              // Thư viện socket cho Windows
#include <ws2tcpip.h>              // Hỗ trợ địa chỉ IPv4/IPv6
#pragma comment(lib, "ws2_32.lib") // Liên kết thư viện Winsock2

std::array<uint8_t, 4> getLocalIPWin()
{
    // Khởi tạo Winsock (Windows Socket API)
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData); // Yêu cầu phiên bản Winsock 2.2

    // Lấy tên máy (hostname)
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) // Nếu lỗi khi lấy hostname
    {
        WSACleanup();        // Dọn dẹp Winsock
        return {0, 0, 0, 0}; // Trả về IP mặc định 0.0.0.0
    }

    // Cấu hình thông tin tìm kiếm địa chỉ IP
    struct addrinfo hints = {}, *info, *p;
    hints.ai_family = AF_INET; // Chỉ lấy địa chỉ IPv4

    // Lấy danh sách địa chỉ IP tương ứng với hostname
    if (getaddrinfo(hostname, NULL, &hints, &info) != 0)
    {
        WSACleanup();        // Dọn dẹp Winsock nếu lỗi
        return {0, 0, 0, 0}; // Trả về IP mặc định 0.0.0.0
    }

    // Khởi tạo mảng để lưu địa chỉ IP
    std::array<uint8_t, 4> localIP = {0, 0, 0, 0};

    // Duyệt danh sách địa chỉ IP lấy được
    for (p = info; p != NULL; p = p->ai_next)
    {
        // Ép kiểu địa chỉ về IPv4 (sockaddr_in)
        struct sockaddr_in *addr = (struct sockaddr_in *)p->ai_addr;

        // Sao chép 4 byte địa chỉ IP vào mảng localIP
        std::memcpy(localIP.data(), &addr->sin_addr, 4);

        // Lấy địa chỉ IP đầu tiên và thoát vòng lặp
        break;
    }

    // Giải phóng bộ nhớ đã cấp phát cho danh sách địa chỉ
    freeaddrinfo(info);

    // Dọn dẹp Winsock
    WSACleanup();

    // Trả về địa chỉ IP của máy tính dưới dạng mảng 4 byte
    return localIP;
}
#else                   // Nếu không phải Windows (giả định Linux)
#include <sys/types.h>  // Kiểu dữ liệu hệ thống
#include <sys/socket.h> // Thư viện socket trên Linux
#include <netdb.h>      // Hỗ trợ DNS và địa chỉ mạng
#include <unistd.h>     // Hỗ trợ thao tác với hệ thống (close socket)
#include <arpa/inet.h>  // Chuyển đổi địa chỉ IP giữa dạng nhị phân và chuỗi
#include <ifaddrs.h>
std::array<uint8_t, 4> getLocalIPLinux()
{
    struct ifaddrs *ifaddr, *ifa;
    std::array<uint8_t, 4> localIP = {0, 0, 0, 0};

    // Lấy danh sách các interface mạng
    if (getifaddrs(&ifaddr) == -1)
    {
        return {0, 0, 0, 0}; // Trả về 0.0.0.0 nếu lỗi
    }

    // Duyệt qua danh sách interface
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        // Chỉ lấy địa chỉ IPv4
        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;

            // Bỏ qua địa chỉ loopback (127.0.0.1)
            if (std::strcmp(ifa->ifa_name, "ens33") == 0) // Chỉ lấy eth0, wlan0, enpXsY, v.v.
            {
                std::memcpy(localIP.data(), &addr->sin_addr, 4);
                break; // Lấy địa chỉ IP đầu tiên không phải loopback rồi thoát
            }
        }
    }

    // Giải phóng danh sách interface
    freeifaddrs(ifaddr);

    return localIP;
}
#endif

std::array<uint8_t, 4> getLocalIP()
{
#ifdef _WIN32
    return getLocalIPWin();
#elif defined(__linux__)
    return getLocalIPLinux();
#else
    return {0, 0, 0, 0}; // Nếu không xác định được hệ điều hành
#endif
}