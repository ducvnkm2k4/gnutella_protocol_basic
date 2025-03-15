#include "network.h"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <array>

#ifdef _WIN32
// Nếu đang sử dụng Windows, bao gồm thư viện Winsock
#include <winsock2.h>
#include <ws2tcpip.h>

// Hàm lắng nghe kết nối từ client
void listenConnect(int port)
{
    // Khởi tạo Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        perror("WSAStartup failed"); // Nếu khởi tạo thất bại
        exit(EXIT_FAILURE);          // Thoát chương trình
    }

    int server_fd, new_socket;     // server_fd là socket của server, new_socket là socket của client
    struct sockaddr_in address;    // Cấu trúc địa chỉ của server
    int addrlen = sizeof(address); // Kích thước của cấu trúc địa chỉ

    // Tạo một socket TCP (SOCK_STREAM) sử dụng giao thức IPv4 (AF_INET)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        perror("Socket failed"); // Nếu tạo socket thất bại
        WSACleanup();            // Dọn dẹp Winsock
        exit(EXIT_FAILURE);      // Thoát chương trình
    }

    // Cấu hình thông tin địa chỉ cho server
    address.sin_family = AF_INET;         // Giao thức IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Lắng nghe trên mọi địa chỉ IP
    address.sin_port = htons(port);       // Thiết lập cổng lắng nghe

    // Gắn socket với địa chỉ và cổng
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
    {
        perror("Bind failed");  // Nếu bind thất bại
        closesocket(server_fd); // Đóng socket
        WSACleanup();           // Dọn dẹp Winsock
        exit(EXIT_FAILURE);     // Thoát chương trình
    }

    // Bắt đầu lắng nghe kết nối đến từ client (tối đa 3 kết nối trong hàng đợi)
    if (listen(server_fd, 3) == SOCKET_ERROR)
    {
        perror("Listen failed"); // Nếu listen thất bại
        closesocket(server_fd);  // Đóng socket
        WSACleanup();            // Dọn dẹp Winsock
        exit(EXIT_FAILURE);      // Thoát chương trình
    }
    std::cout << "Server is listening on port " << port << "..." << std::endl;

    // Vòng lặp vô hạn để server tiếp tục lắng nghe và duy trì kết nối
    while (true)
    {
        // Chấp nhận kết nối từ client
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        std::cout << "Client connected" << std::endl;

        // Vòng lặp để xử lý dữ liệu từ client
        while (true)
        {
            char buffer[1024] = {0};
            int valread = recv(new_socket, buffer, sizeof(buffer), 0);

            if (valread == 0)
            {
                std::cout << "Client disconnected" << std::endl;
                break; // Client ngắt kết nối
            }
            else if (valread < 0)
            {
                perror("Receive failed");
                break; // Lỗi khi nhận dữ liệu
            }
            else
            {
                std::cout << "Received from client: " << buffer << std::endl;
                // Gửi lại dữ liệu cho client (nếu cần)
                send(new_socket, buffer, valread, 0);
            }
        }

        // Đóng socket của client sau khi kết thúc giao tiếp
        close(new_socket);
    }

    // Đóng socket server sau khi thoát khỏi vòng lặp (tuy nhiên trong thực tế, server vẫn chạy vô thời hạn)
    close(server_fd);
}

// Hàm kết nối tới server (client)
void connectPeer(string peerIP, int port)
{
    // Khởi tạo Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        perror("WSAStartup failed"); // Nếu khởi tạo thất bại
        exit(EXIT_FAILURE);          // Thoát chương trình
    }

    int sock;
    struct sockaddr_in server_address;

    // Tạo một socket TCP (SOCK_STREAM) sử dụng giao thức IPv4 (AF_INET)
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        perror("Socket failed"); // Nếu tạo socket thất bại
        WSACleanup();            // Dọn dẹp Winsock
        exit(EXIT_FAILURE);      // Thoát chương trình
    }

    // Cấu hình thông tin địa chỉ của server mà client muốn kết nối
    server_address.sin_family = AF_INET;                // Giao thức IPv4
    server_address.sin_port = htons(port);              // Cổng 8080
    server_address.sin_addr.s_addr = inet_addr(peerIP); // Địa chỉ IP của server (localhost)

    // Kết nối tới server qua socket
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        perror("Connection failed"); // Nếu kết nối thất bại
        closesocket(sock);           // Đóng socket
        WSACleanup();                // Dọn dẹp Winsock
        exit(EXIT_FAILURE);          // Thoát chương trình
    }

    std::cout << "Connected to server" << std::endl;

    // Nhận dữ liệu từ server
    char buffer[1024] = {0};
    int valread = recv(sock, buffer, sizeof(buffer), 0);
    if (valread > 0)
    {
        std::cout << "Server says: " << buffer << std::endl; // In dữ liệu nhận được từ server
    }

    // Đóng socket sau khi giao tiếp xong
    closesocket(sock);

    // Dọn dẹp Winsock
    WSACleanup();
}

#elif defined(__linux__)
// Nếu đang sử dụng Linux, bao gồm các thư viện POSIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Hàm lắng nghe kết nối từ client
void listenConnect(std::array<uint8_t, 4> ipv4, int port)
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Tạo socket TCP (SOCK_STREAM) sử dụng giao thức IPv4 (AF_INET)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed"); // Nếu tạo socket thất bại
        exit(EXIT_FAILURE);      // Thoát chương trình
    }

    // Cấu hình thông tin địa chỉ cho server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Gắn socket với địa chỉ và cổng (bind)
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed"); // Nếu bind thất bại
        exit(EXIT_FAILURE);    // Thoát chương trình
    }

    // Bắt đầu lắng nghe kết nối đến (tối đa 3 kết nối trong hàng đợi)
    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed"); // Nếu listen thất bại
        exit(EXIT_FAILURE);      // Thoát chương trình
    }
    std::cout << "Server is listening on port " << port << "..." << std::endl;

    // Vòng lặp vô hạn để server tiếp tục lắng nghe và duy trì kết nối
    while (true)
    {
        // Chấp nhận kết nối từ client
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        std::cout << "Client connected" << std::endl;

        // Vòng lặp để xử lý dữ liệu từ client
        while (true)
        {
            char buffer[1024] = {0};
            int valread = recv(new_socket, buffer, sizeof(buffer), 0);

            if (valread == 0)
            {
                std::cout << "Client disconnected" << std::endl;
                break; // Client ngắt kết nối
            }
            else if (valread < 0)
            {
                perror("Receive failed");
                break; // Lỗi khi nhận dữ liệu
            }
            else
            {
                std::cout << "Received from client: " << buffer << std::endl;
                // Gửi lại dữ liệu cho client (nếu cần)
                send(new_socket, buffer, valread, 0);
            }
        }

        // Đóng socket của client sau khi kết thúc giao tiếp
        close(new_socket);
    }

    // Đóng socket server sau khi thoát khỏi vòng lặp (tuy nhiên trong thực tế, server vẫn chạy vô thời hạn)
    close(server_fd);
}

// Hàm kết nối tới server (client)
void connectPeer(string peerIP, int port)
{
    int sock;
    struct sockaddr_in server_address;

    // Tạo socket TCP (SOCK_STREAM) sử dụng giao thức IPv4 (AF_INET)
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket failed"); // Nếu tạo socket thất bại
        exit(EXIT_FAILURE);      // Thoát chương trình
    }

    // Cấu hình thông tin địa chỉ của server mà client muốn kết nối
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(peerIP);

    // Kết nối tới server qua socket
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Connection failed"); // Nếu kết nối thất bại
        exit(EXIT_FAILURE);          // Thoát chương trình
    }

    std::cout << "Connected to server" << std::endl;

    // Nhận dữ liệu từ server
    char buffer[1024] = {0};
    int valread = recv(sock, buffer, sizeof(buffer), 0);
    if (valread > 0)
    {
        std::cout << "Server says: " << buffer << std::endl;
    }

    // Đóng socket sau khi giao tiếp xong
    close(sock);
}

#endif
