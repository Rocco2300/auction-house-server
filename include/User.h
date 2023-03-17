#pragma once

#include <Field.h>

#include <unordered_map>

#include <sqlite3.h>

class User
{
private:
    int m_userId{};
    std::string m_username{};
    std::string m_password{};

#pragma FIELDS
    std::unordered_map<std::string, Field> fields = {
            {"userId", Field(m_userId)},
            {"username", Field(m_username)},
            {"password", Field(m_password)}
    };

public:
    User() = default;
    User(std::string username, std::string password = "");

    void operator=(sqlite3_stmt* stmt);
    void buildStmt(sqlite3* db, sqlite3_stmt** stmt);

    Field& operator[](const std::string& key);
};