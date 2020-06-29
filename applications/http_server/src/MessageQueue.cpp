#include <MessageQueue.h>
#include <iostream>

MessageQueue::MessageQueue() {
    std::cout << "Message Queue created!" << std::endl;
}

bool MessageQueue::enqueue(const std::string &msg) {
    std::scoped_lock<std::mutex> lock(qMutex);
    queue.push(Message(msg));
    messageConditional.notify_one();
    return true;
}

bool MessageQueue::dequeue(std::string *msg) {
    std::unique_lock<std::mutex> lock(qMutex);
    while (queue.empty()) {
        messageConditional.wait(lock);
    }

    if (queue.empty()) {
        return false;
    }

    Message message(queue.front());
    queue.pop();
    *msg = message.getMessage();
    return true;
};


MessageQueue::Message::Message(const std::string& msg) : message(msg) {}

const std::string& MessageQueue::Message::getMessage() const {
    return message;
}
