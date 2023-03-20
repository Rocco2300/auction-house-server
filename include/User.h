#pragma once

#include "FieldVector.h"

class User {
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
    User() = default;
    User(std::string username, std::string password = "");

    size_t size();

    Field& operator[](size_t index);
    Field& operator[](const std::string& key);
};