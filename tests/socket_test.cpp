#include <gtest/gtest.h>
#include <Socket.h>
using namespace SocketLayer;
TEST(SocketTest, ServerTest){
    Socket server(Endpoint("localhost", 8080));

    EXPECT_FALSE(server.listen());


    EXPECT_TRUE(server.bind());
    EXPECT_FALSE(server.connect().isValid());
    EXPECT_TRUE(server.listen());
    EXPECT_FALSE(server.connect().isValid());
}

TEST(SocketTest, ClientTest){
    Socket client(Endpoint("localhost", 8080));
    EXPECT_FALSE(client.connect().isValid());

    Socket server(Endpoint("localhost", 8080));
    server.bind();
    server.listen();

    EXPECT_TRUE(client.connect().isValid());
    EXPECT_FALSE(client.bind());
    EXPECT_FALSE(client.listen());
}

TEST(SocketTest, IP_and_name){
    Socket client(Endpoint("localhost", 8080));
    Socket server(Endpoint("127.0.0.1", 8080));
    server.bind();
    server.listen();

    EXPECT_TRUE(client.connect().isValid());
}

TEST(SocketTest, WrongAddress) {
    Socket server(Endpoint("localhost", 8080));
    Socket client(Endpoint("192.167.1.24", 8080));

    server.bind();
    server.listen();

    EXPECT_FALSE(client.connect().isValid());
}

TEST(SocketTest, WrongPort) {
    Socket server(Endpoint("localhost", 8080));
    Socket client(Endpoint("localhost", 8081));

    server.bind();
    server.listen();

    EXPECT_FALSE(client.connect().isValid());
}

TEST(SocketTest, TestSimpleSendAndReceive){
    Socket server(Endpoint("localhost", 8080));
    server.bind();
    server.listen();


    Socket client(Endpoint("localhost", 8080));
    Connection c = client.connect();


    std::string msgToSend = "This is a message";
    server.accept().send(msgToSend);

    std::string msg;
    c.recv(msgToSend.size(), &msg);
    EXPECT_EQ(msg, msgToSend);
}

TEST(SocketTest, TryAndReceiveMoreThanSent){
    Socket server(Endpoint("localhost", 8081));
    server.bind();
    server.listen();


    Socket client(Endpoint("localhost", 8081));
    Connection c = client.connect();

    std::string msgToSend = "This is a message";

    server.accept().send(msgToSend);

    std::string msg;
    c.recv(msgToSend.size(), &msg);
    EXPECT_EQ(msg, msgToSend);
}

