#pragma once

#include "Session.h"

class SessionManager {
private:
    std::unordered_map<std::string, SessionPtr> m_sessions;
public:
    SessionManager() = default;

    bool isConnected(std::string key);
    void registerSession(std::string key, SessionPtr session);

    SessionPtr getSession(std::string key);
};