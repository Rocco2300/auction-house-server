#pragma once

#include "Entity.h"

class User : public Entity {
private:
    int         m_userId{};
    std::string m_username{};
    std::string m_password{};

#pragma FIELDS
    FieldVector fields = {
            {"userId", m_userId},
            {"username", m_username},
            {"password", m_password},
    };

public:
    User();
    User(std::string username, std::string password = "");
};