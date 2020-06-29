#include <gtest/gtest.h>
#include <thread>
#include <applications/http_server/include/MessageQueue.h>

using namespace std;

TEST(MsgQueue, SimpleTest){
    MessageQueue q;
    std::thread t([&q](){sleep(5);q.enqueue("gg");});
    q.enqueue("hello");
    q.enqueue("hh");
    string s;

    q.dequeue(&s);
    ASSERT_EQ(s, "hello");


    q.dequeue(&s);
    ASSERT_EQ(s, "hh");


    q.dequeue(&s);
    ASSERT_EQ(s, "gg");


    t.join();
}
