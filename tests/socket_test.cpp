#include <gtest/gtest.h>
#include <include/Socket.h>

TEST(SocketTest, ServerTest){
    Socket server(Endpoint("localhost", 8080));

    EXPECT_FALSE(server.listen());


    EXPECT_TRUE(server.bind());
    EXPECT_FALSE(server.connect());
    EXPECT_TRUE(server.listen());
    EXPECT_FALSE(server.connect());
}

TEST(SocketTest, ClientTest){
    Socket client(Endpoint("localhost", 8080));
    EXPECT_FALSE(client.connect());

    Socket server(Endpoint("localhost", 8080));
    server.bind();
    server.listen();

    EXPECT_TRUE(client.connect());
    EXPECT_FALSE(client.bind());
    EXPECT_FALSE(client.listen());
}

TEST(SocketTest, IP_and_name){
    Socket client(Endpoint("localhost", 8080));
    Socket server(Endpoint("127.0.0.1", 8080));
    server.bind();
    server.listen();

    EXPECT_TRUE(client.connect());
}

TEST(SocketTest, WrongAddress) {
    Socket server(Endpoint("localhost", 8080));
    Socket client(Endpoint("192.167.1.24", 8080));

    server.bind();
    server.listen();

    EXPECT_FALSE(client.connect());
}

TEST(SocketTest, WrongPort) {
    Socket server(Endpoint("localhost", 8080));
    Socket client(Endpoint("localhost", 8081));

    server.bind();
    server.listen();

    EXPECT_FALSE(client.connect());
}

TEST(SocketTest, TestSimpleSendAndReceive){
    Socket server(Endpoint("localhost", 8080));
    server.bind();
    server.listen();


    Socket client(Endpoint("localhost", 8080));
    client.connect();

    server.accept();
    std::string msgToSend = "This is a message";
    server.send(msgToSend);

    std::string msg;
    client.recv(msgToSend.size(), &msg);
    EXPECT_EQ(msg, msgToSend);
}

TEST(SocketTest, TryAndReceiveMoreThanSent){
    Socket server(Endpoint("localhost", 8081));
    server.bind();
    server.listen();


    Socket client(Endpoint("localhost", 8081));
    client.connect();

    server.accept();
    std::string msgToSend = "This is a message";
    server.send(msgToSend);

    std::string msg;
    client.recv(9999999, &msg);
    EXPECT_EQ(msg, msgToSend);
}

