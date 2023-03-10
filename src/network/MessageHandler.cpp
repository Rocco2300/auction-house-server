#include "MessageHandler.h"

#include <iostream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

    json j = json::parse(request);

    std::cout << j["action"] << std::endl;

    auto action = j["action"];
    if (action == "query") {
        return "queried";
    } else if (action == "buy") {
        return "bought";
    } else if (action == "post") {
        return "posted";
    } else if (action == "order") {
        return "ordered";
    } else if (action == "login") {
        std::string username = j["data"]["username"];
        std::string password = j["data"]["password"];

        std::cout << username << ' ' << password << '\n';
    }
    return "invalid request";
}