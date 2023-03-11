#pragma once

#include <string>

#include <sqlite3.h>

class User
{
private:
    int m_userId;
    std::string m_username;
    std::string m_password;

public:
    User() = default;
    User(std::string username, std::string password = "");

    void operator=(sqlite3_stmt* stmt);
    void buildStmt(sqlite3* db, sqlite3_stmt** stmt);
};