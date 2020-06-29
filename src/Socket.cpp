#include <Socket.h>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <poll.h>

Endpoint::Endpoint(const std::string& address, port_t port) : address(address), port(port) {}

port_t Endpoint::getPort() const {
    return port;
}

const std::string& Endpoint::getAddress() const {
    return address;
}

Socket::Socket(const Endpoint &endpoint) : endpoint(endpoint), is_bound(false), is_connected(false) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        perror("Invalid socket");
}

Socket::~Socket() {
    std::cout << "Socket " << sock << " destroyed!" << std::endl;
    close(sock);
}

bool Socket::bind() {
    if (is_connected) {
        std::cerr << "Socket is already connected so can't bind" << std::endl;
        return false;
    }

    int on = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on)) < 0) {
        perror("setsockopt() failed");
        return false;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons((uint16_t) endpoint.getPort());

    if (::bind(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Failed to bind socket.");
        return false;
    }
    is_bound = true;
    return true;
}

bool Socket::listen() {
    if (!is_bound) {
        std::cerr << "You must bind before listening" << std::endl;
        return false;
    }

    if (::listen(sock, MAX_CLIENTS) < 0){
        perror("Socket listen failed");
        return false;
    }

    std::cout  << "Socket " << sock << " is listening on port " << endpoint.getPort() << std::endl;
    return true;
}


Connection Socket::connect() {
    if (is_bound)  {
        std::cerr << "Socket is already bound so can't connect" << std::endl;
        return false;
    }

    struct addrinfo* res;
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (::getaddrinfo(endpoint.getAddress().c_str(), std::to_string(endpoint.getPort()).c_str(), &hints, &res) != 0) {
        fprintf(stderr,"Getaddrinfo failed. Invalid ip/port");
        //return false;
    }

    if (::connect(sock, res->ai_addr, res->ai_addrlen) < 0){
        perror("connect");
        freeaddrinfo(res);
        //return false;
    }

    freeaddrinfo(res);
    is_connected = true;
    return Connection(dup(sock));

}


Connection Socket::accept() {
//    if (!is_bound)
//        return false;

    auto temp = ::accept(sock, nullptr, nullptr);
    return Connection(temp);
}

int Socket::getSock() const {
    return sock;
}

//bool Socket::send(const std::string &msg) {
//    std::string_view msg_view(msg);
//
//    while (!msg_view.empty()) {
//        int amount_sent = ::write(sock, msg_view.data(), msg_view.length());
//        if (amount_sent < 0) {
//            if (errno == EINTR) {
//                continue;
//            }
//            perror("Unexpected error while sending");
//            return false;
//        }
//        msg_view.remove_prefix(amount_sent);
//    }
//    return true;
//}

//bool Socket::recv(int msg_len, std::string *msg) {
//    struct pollfd pfd;
//    pfd.fd = sock;
//    pfd.events = POLLIN;
//    char* buf = new char[msg_len + 1];
//    char* ptr = buf;
//    while (msg_len > 0) {
//        if (poll(&pfd, 1, 3000) == 0)
//            break;
//        int amount_read = ::recv(sock, ptr, msg_len, 0);
//        msg_len -= amount_read;
//        ptr += amount_read;
//    }
//    *msg = buf;
//    delete[] buf;
//    return true;
//}


bool Connection::recv(int msg_len, std::string *msg) {
    struct pollfd pfd;
    pfd.fd = sock;
    pfd.events = POLLIN;
    char* buf = new char[msg_len + 1];
    char* ptr = buf;
    while (msg_len > 0) {
        if (poll(&pfd, 1, 3000) == 0)
            break;
        int amount_read = ::recv(sock, ptr, msg_len, 0);
        msg_len -= amount_read;
        ptr += amount_read;
    }
    *msg = buf;
    delete[] buf;
    return true;
}

bool Connection::send(const std::string &msg) {
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

Connection::Connection(int sock) : sock(sock){}

Connection::Connection(Connection &&conn) {
    sock = std::move(conn.sock);
}

int Connection::getSock() const {
    return sock;
}
