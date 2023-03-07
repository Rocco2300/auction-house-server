#include "SessionManager.h"

#include <iostream>
#include <stdexcept>

bool SessionManager::isConnected(std::string key) {
    return m_sessions.find(key) != m_sessions.end();
}

void SessionManager::disconnectSession(std::string key) {
    if (!isConnected(key)) {
        std::stringstream errMsg;
        errMsg << "Error: User " << key << " not found!";
        throw std::out_of_range(errMsg.str());
    }

    m_sessions.erase(key);
}

void SessionManager::registerSession(std::string key, SessionPtr session) {
    m_sessions.insert({key, session});

    for (const auto& [username, ptr] : m_sessions) {
        std::cout << username << ' ' << ptr.get() << '\n';
    }
}

SessionPtr SessionManager::getSession(std::string key) {
    if (!isConnected(key)) {
        std::stringstream errMsg;
        errMsg << "Error: User " << key << " not found!";
        throw std::out_of_range(errMsg.str());
    }

    return m_sessions[key];
}