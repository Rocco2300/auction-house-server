#pragma once

#include "Session.h"

class Request
{
private:
    std::weak_ptr<Session> m_sender{};
    std::string m_data{};
    std::string m_response{};

public:
    Request(std::shared_ptr<Session> sender, std::string data);

    const std::string& getData();

    void setResponse(const std::string& response);
    void sendResponse();
};