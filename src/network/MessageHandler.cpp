#include "MessageHandler.h"


void MessageHandler::enqueueRequest(Message message, MessageCallback callback) {
    m_requestQueue.push({message, callback});
}

void MessageHandler::run() {
    m_thread = std::thread(&MessageHandler::handleMessages, this);
}

void MessageHandler::handleMessages() {
    while (true) {
        if (!m_requestQueue.empty()) {
            auto [message, callback] = m_requestQueue.front();
            auto response            = getResponse(message);

            callback(response);
            m_requestQueue.pop();
        }
    }
}

// TODO: placeholder
Message MessageHandler::getResponse(Message request) {
    if (request == "query") {
        return "queried";
    } else if (request == "buy") {
        return "bought";
    } else if (request == "post") {
        return "posted";
    } else if (request == "order") {
        return "ordered";
    }
    return "invalid request";
}