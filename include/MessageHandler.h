#pragma once

#include "SessionManager.h"

#include <functional>
#include <queue>
#include <string>
#include <thread>
#include <utility>

using Message         = std::string;
using MessageCallback = std::function<void(std::string)>;

class MessageHandler {
private:
    std::thread                                     m_thread;
    std::queue<std::pair<Message, MessageCallback>> m_requestQueue;

public:
    MessageHandler() = default;

    void run();
    void enqueueRequest(Message message, MessageCallback callback);

private:
    void    handleMessages();
    Message getResponse(Message request);
};