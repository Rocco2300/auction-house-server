#include "MessageHandler.h"
#include "Db.h"
#include "User.h"

#include <iostream>

#include <fmt/core.h>
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

// TODO: this is temporary, will be moved in db object
static void buildStatementFromEntity(
        User& user, sqlite3* db, sqlite3_stmt** stmt
) {
    std::string query = fmt::format(
            "SELECT * FROM users WHERE username = '{}'", user["username"]
    );
    int ret = sqlite3_prepare_v2(db, query.c_str(), query.length(), stmt, 0);
    if (ret) {
        std::cerr << "Prepare failed\n";
        return;
    }
}

// TODO: this is temporary, will be moved in db object
static void readEntityFromStatement(User& user, sqlite3_stmt* stmt) {
    for (int i = 0; i < user.size(); i++) {
        user[i] = sqlite3_column_blob(stmt, i);
    }
}

bool MessageHandler::handleLogin(json jsonRequest) {
    sqlite3* dbHandle;

    int   ret = sqlite3_open("../db/auction_house.db", &dbHandle);
    char* errorMessage;

    if (ret) {
        std::cerr << "Error opening db\n";
        return false;
    }


    User user;
    user["username"] = jsonRequest["data"]["username"];
    ReadDbEntity(dbHandle, user);

    std::cout << user["userId"] << ' ' << user["username"] << '\n';

    json entJson = user;
    std::cout << entJson.dump(4) << '\n';

    return true;
}