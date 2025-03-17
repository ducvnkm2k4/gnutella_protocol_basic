#include "network.h"
#include "messageManager.h"
#include "messagePacket.h"
#include <cstring>
#include <iostream>
#include <array>
#include <vector>

#ifdef _WIN32
// Nếu đang sử dụng Windows, bao gồm thư viện Winsock
#include <winsock2.h>
#include <ws2tcpip.h>

void listenMessageUDP(int port)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sock, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    std::cout << "Listening for UDP messages on port " << port << "...\n";

    while (true)
    {
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        char buffer[1024];

        int recvLen = recvfrom(sock, buffer, sizeof(buffer), 0, (sockaddr *)&clientAddr, &clientAddrLen);
        if (recvLen == SOCKET_ERROR)
        {
            std::cerr << "recvfrom failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        buffer[recvLen] = '\0';
        std::cout << "Received UDP message: " << buffer << std::endl;

        // handleMessageUDP(buffer, clientAddr);
    }

    closesocket(sock);
    WSACleanup();
}

void handleMessageUDP(const char *message, sockaddr_in &clientAddr)
{
    std::cout << "Processing message: " << message << std::endl;

    if (strcmp(message, "PING") == 0)
    {
        // sendPongMessage(clientAddr);
    }
}

// Hàm lắng nghe kết nối từ client
void listenConnectTCP(int port)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        std::cerr << "Socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Sử dụng setsockopt để tránh lỗi "Address already in use"
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << port << "..." << std::endl;

    while (true)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) == INVALID_SOCKET)
        {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            continue; // Không nên thoát hẳn server nếu chỉ một kết nối bị lỗi
        }

        std::cout << "Client connected" << std::endl;

        std::vector<char> buffer(1024);
        int valread;
        while ((valread = recv(new_socket, buffer.data(), buffer.size(), 0)) > 0)
        {
            std::cout << "Received from client: " << buffer.data() << std::endl;
            send(new_socket, buffer.data(), valread, 0);
        }

        if (valread == 0)
        {
            std::cout << "Client disconnected" << std::endl;
        }
        else
        {
            std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
        }

        closesocket(new_socket);
    }

    closesocket(server_fd);
    WSACleanup();
}

// Hàm kết nối tới server (client)
void sendMessageTCP(const std::string peerIP, int port)
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
    server_address.sin_family = AF_INET;                        // Giao thức IPv4
    server_address.sin_port = htons(port);                      // Cổng 8080
    server_address.sin_addr.s_addr = inet_addr(peerIP.c_str()); // Địa chỉ IP của server (localhost)

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
void sendPingMessage(const std::string &peerIP, int port)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        return;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(peerIP.c_str());

    const char *msg = "PING";
    MessagePacket ping = pingMessage();
    sendto(sock, msg, strlen(msg), 0, (sockaddr *)&serverAddr, sizeof(serverAddr));

    std::cout << "Sent PING message to " << peerIP << ":" << port << std::endl;

    closesocket(sock);
    WSACleanup();
}

void sendPongMessage(sockaddr_in &clientAddr)
{
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed\n";
        return;
    }

    const char *msg = "PONG";
    sendto(sock, msg, strlen(msg), 0, (sockaddr *)&clientAddr, sizeof(clientAddr));

    std::cout << "Sent PONG response\n";

    closesocket(sock);
}
void sendQueryMessage()
{
}
void sendQueryHitMessage()
{
}
#elif defined(__linux__)
// Nếu đang sử dụng Linux, bao gồm các thư viện POSIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void listenMessageUDP(int port)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("Socket creation failed");
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Bind failed");
        close(sock);
        return;
    }

    std::cout << "Listening for UDP messages on port " << port << "...\n";

    while (true)
    {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        char buffer[1024];

        int recvLen = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (recvLen < 0)
        {
            perror("recvfrom failed");
            continue;
        }

        buffer[recvLen] = '\0';
        std::cout << "Received UDP message: " << buffer << std::endl;

        handleMessageUDP(buffer, clientAddr);
    }

    close(sock);
}

void handleMessageUDP(const char *message, sockaddr_in &clientAddr)
{
    std::cout << "Processing message: " << message << std::endl;

    if (strcmp(message, "PING") == 0)
    {
        sendPongMessage(clientAddr);
    }
}
// Hàm lắng nghe kết nối từ client
void listenConnectTCP(int port)
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << port << "..." << std::endl;

    while (true)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept failed");
            continue; // Không nên thoát chương trình khi accept thất bại
        }

        std::cout << "Client connected" << std::endl;

        std::vector<char> buffer(1024);
        int valread;
        while ((valread = recv(new_socket, buffer.data(), buffer.size(), 0)) > 0)
        {
            std::cout << "Received from client: " << buffer.data() << std::endl;
            send(new_socket, buffer.data(), valread, 0);
        }

        if (valread == 0)
        {
            std::cout << "Client disconnected" << std::endl;
        }
        else
        {
            perror("Receive failed");
        }

        close(new_socket);
    }

    close(server_fd);
}

// Hàm kết nối tới server (client)
void sendMessageTCP(const std::string peerIP, int port)
{
    int sock;
    struct sockaddr_in server_address;

    // Tạo socket TCP (SOCK_STREAM) sử dụng giao thức IPv4 (AF_INET)
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        g
        {
            perror("Socket failed"); // Nếu tạo socket thất bại
            exit(EXIT_FAILURE);      // Thoát chương trình
        }

    // Cấu hình thông tin địa chỉ của server mà client muốn kết nối
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(peerIP.c_str());

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
void sendPingMessage(const std::string &peerIP, int port)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("Socket creation failed");
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, peerIP.c_str(), &serverAddr.sin_addr);

    const char *msg = "PING";
    sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    std::cout << "Sent PING message to " << peerIP << ":" << port << std::endl;

    close(sock);
}

void sendPongMessage(sockaddr_in &clientAddr)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("Socket creation failed");
        return;
    }

    const char *msg = "PONG";
    sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));

    std::cout << "Sent PONG response\n";

    close(sock);
}
void sendQueryMessage()
{
}
void sendQueryHitMessage()
{
}
#endif
