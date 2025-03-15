#ifndef GET_LOCAL_IP
#define GET_LOCAL_IP

#include <array>   // Sử dụng std::array để lưu địa chỉ IP
#include <cstdint> // Định nghĩa kiểu uint8_t (8-bit không dấu)

#ifdef _WIN32
std::array<uint8_t, 4> getLocalIPWin();
#else
std::array<uint8_t, 4> getLocalIPLinux();
#endif

std::array<uint8_t, 4> getLocalIP();

#endif // GET_LOCAL_IP
