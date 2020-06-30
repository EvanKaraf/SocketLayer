#ifndef HTTP_SERVER_SOCKET_H
#define HTTP_SERVER_SOCKET_H

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <vector>

namespace SocketLayer {


typedef int port_t;
const int MAX_CLIENTS = 10;

class Endpoint {
    std::string address;
public:
    const std::string& getAddress() const;
private:
    port_t port;
public:
    port_t getPort() const;

public:
    Endpoint(const std::string& address, port_t port);
};

class Connection {
    int sock;
public:
    Connection(int sock);

    Connection(Connection& conn) = delete;

    Connection(Connection&& conn);

    void shutdown();

    int getSock() const;

    bool isValid() const;

    bool recv(int msg_len, std::string* msg) const;

    bool send(const std::string& msg) const;
};


class Socket {
    Endpoint endpoint;
    int sock;
    bool is_bound;
    bool is_connected;
public:
    Socket(const Endpoint& endpoint);

    virtual ~Socket();

    bool bind();

    bool listen();

    Connection connect();

    Connection accept();

    int getSock() const;
};

// Implementation
inline Endpoint::Endpoint(const std::string& address, port_t port) : address(address), port(port) {}

inline port_t Endpoint::getPort() const {
    return port;
}

inline const std::string& Endpoint::getAddress() const {
    return address;
}


inline Socket::Socket(const Endpoint& endpoint) : endpoint(endpoint), is_bound(false), is_connected(false) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        perror("Invalid socket");
}

inline Socket::~Socket() {
    std::cout << "Socket " << sock << " destroyed!" << std::endl;
    close(sock);
}

inline bool Socket::bind() {
    if (is_connected) {
        std::cerr << "Socket is already connected so can't bind" << std::endl;
        return false;
    }

    int on = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*) &on, sizeof(on)) < 0) {
        perror("setsockopt() failed");
        return false;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons((uint16_t) endpoint.getPort());

    if (::bind(sock, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("Failed to bind socket.");
        return false;
    }
    is_bound = true;
    return true;
}

inline bool Socket::listen() {
    if (!is_bound) {
        std::cerr << "You must bind before listening" << std::endl;
        return false;
    }

    if (::listen(sock, MAX_CLIENTS) < 0) {
        perror("Socket listen failed");
        return false;
    }

    std::cout << "Socket " << sock << " is listening on port " << endpoint.getPort() << std::endl;
    return true;
}


inline Connection Socket::connect() {
    if (is_bound) {
        std::cerr << "Socket is already bound so can't connect" << std::endl;
        return Connection(-1);
    }

    struct addrinfo* res;
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (::getaddrinfo(endpoint.getAddress().c_str(), std::to_string(endpoint.getPort()).c_str(), &hints, &res) !=
        0) {
        fprintf(stderr, "Getaddrinfo failed. Invalid ip/port");
        return Connection(-1);
    }

    if (::connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
        perror("connect");
        freeaddrinfo(res);
        return Connection(-1);
    }

    freeaddrinfo(res);
    is_connected = true;
    return Connection(dup(sock));

}


inline Connection Socket::accept() {
    if (!is_bound)
        return Connection(-1);

    auto temp = ::accept4(sock, nullptr, nullptr, O_NONBLOCK);
    return Connection(temp);
}

inline int Socket::getSock() const {
    return sock;
}

inline Connection::Connection(int sock) : sock(dup(sock)) {}

inline Connection::Connection(Connection&& conn) {
    sock = dup(conn.sock);
}

inline bool Connection::recv(int msg_len, std::string* msg) const {
    struct pollfd pfd{};
    pfd.fd = sock;
    pfd.events = POLLIN;
    const unsigned int MAX_BUF_LENGTH = 4096;
    std::vector<char> buffer(MAX_BUF_LENGTH + 1);
    while (msg_len > 0 || msg_len == -1) {
        if (poll(&pfd, 1, 1000) <= 0)
            break;

        int amount_read = ::recv(sock, &buffer[0], buffer.size() - 1, 0);
        msg_len -= amount_read;
        buffer[amount_read] = '\0';
        msg->append(buffer.begin(), buffer.begin() + amount_read);
        buffer.clear();
    }
    std::cout << "-------Message--------" << std::endl;
    std::cout << *msg << std::endl;
    std::cout << "-----Message End------" << std::endl;
    return true;
}

inline bool Connection::send(const std::string& msg) const {
    std::string_view msg_view(msg);

    while (!msg_view.empty()) {
        int amount_sent = ::write(sock, msg_view.data(), msg_view.length());
        if (amount_sent < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("Unexpected error while sending");
            return false;
        }
        msg_view.remove_prefix(amount_sent);
    }
    return true;
}


inline int Connection::getSock() const {
    return sock;
}

inline bool Connection::isValid() const {
    return sock != -1;
}

inline void Connection::shutdown() {
    ::shutdown(sock, 2);
    sock = -1;
}


} // namespace
#endif //HTTP_SERVER_SOCKET_H
