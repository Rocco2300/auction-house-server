#include "SessionManager.h"

#include <stdexcept>

bool SessionManager::isConnected(std::string key) {
    return m_sessions.find(key) != m_sessions.end();
}

void SessionManager::registerSession(std::string key, SessionPtr session) {
    m_sessions.insert({key, session});
}

SessionPtr SessionManager::getSession(std::string key) {
    if (!isConnected(key)) {
        std::stringstream errMsg;
        errMsg << "Error: User " << key << " not found!";
        throw std::out_of_range(errMsg.str());
    }

    return m_sessions[key];
}