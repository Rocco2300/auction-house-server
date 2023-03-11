#include "MessageHandler.h"
#include "User.h"

#include <iostream>

#include <sqlite3.h>

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
        handleLogin(j);
        return "login";
    }
    return "invalid request";
}

bool MessageHandler::handleLogin(json jsonRequest) {
    sqlite3*    dbHandle;
    int         ret = sqlite3_open("../db/auction_house.db", &dbHandle);
    char*       errorMessage;
    std::string data = "CALLBACK";

    if (ret) {
        std::cerr << "Error opening db\n";
        return false;
    }

    sqlite3_stmt* stmt;

    User user("admin");
    user.buildStmt(dbHandle, &stmt);

//    std::string query = "SELECT * FROM users WHERE username = ?";
//    ret = sqlite3_prepare_v2(dbHandle, query.c_str(), query.length(), &stmt, 0);
//    if (ret) {
//        std::cerr << "Prepare failed\n";
//    }
//
//    std::string username = jsonRequest["data"]["username"];
//    std::cout << "Bind ret: "
//              << sqlite3_bind_text(
//                         stmt, 1, username.c_str(), username.length(), nullptr
//                 )
//              << '\n';

    sqlite3_step(stmt);

    User user2;
    user2 = stmt;

    sqlite3_finalize(stmt);

    return true;
}