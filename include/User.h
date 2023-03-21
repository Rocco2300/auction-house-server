#pragma once

#include "Entity.h"

class User : public Entity {
private:
    int         m_userId{};
    std::string m_username{};
    std::string m_password{};

    std::string tableName = "users";
    std::string entityName = "user";
#pragma FIELDS
    FieldVector fields = {
            {"userId", m_userId},
            {"username", m_username},
            {"password", m_password},
    };

public:
    User();
    explicit User(std::string username, std::string password = "");

    User& operator=(const json& other);
};