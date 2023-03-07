#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>

class Session;

using SessionPtr = std::shared_ptr<Session>;

class SessionManager {
private:
    std::unordered_map<std::string, SessionPtr> m_sessions;
public:
    SessionManager() = default;

    bool isConnected(std::string key);
    void registerSession(std::string key, SessionPtr session);

    SessionPtr getSession(std::string key);
};