
include_directories(include)
set(SOURCE_FILES_HTTP_SERVER src/HTTP_Server.cpp src/MessageQueue.cpp)

add_executable(Server ${SOURCE_FILES_HTTP_SERVER} main.cpp)
target_link_libraries(Server  Threads::Threads)

add_executable(MessageQueueTest tests/message_queue_test.cpp ${SOURCE_FILES_HTTP_SERVER})
target_link_libraries(MessageQueueTest gtest_main Threads::Threads)