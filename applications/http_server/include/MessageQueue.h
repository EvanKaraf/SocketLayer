#ifndef HTTP_SERVER_MESSAGEQUEUE_H
#define HTTP_SERVER_MESSAGEQUEUE_H


#include <array>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

const int64_t  MESSAGE_SIZE = 1024 ;

class MessageQueue {
    class Message {
        std::string message;
    public:
        Message(const std::string &msg);

        const std::string& getMessage() const;
    };

    std::queue<Message> queue;
    std::mutex qMutex;
    std::condition_variable messageConditional;

public:
    MessageQueue();
    bool enqueue(const std::string& msg);
    bool dequeue(std::string* msg);
};


#endif //HTTP_SERVER_MESSAGEQUEUE_H
