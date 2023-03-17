#pragma once

#include <Field.h>

#include <unordered_map>

class User {
private:
    int         m_userId{};
    std::string m_username{};
    std::string m_password{};

#pragma FIELDS
    std::vector<std::string> fieldKeys = {
            "userId",
            "username",
            "password",
    };

    std::unordered_map<std::string, Field> fields = {
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