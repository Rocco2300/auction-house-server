#include "Request.h"

Request::Request(std::shared_ptr<Session> sender, std::string data)
    : m_sender(sender)
    , m_data(data) {}

const std::string& Request::getData() { return m_data; }

void Request::setResponse(const std::string& response) {
    m_response = response;
}

void Request::sendResponse() {
    if (m_sender.expired())
        return;

    if (m_response == "")
        return;

    m_sender.lock()->sendMessage(m_response);
}