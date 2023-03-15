#include "User.h"

#include "Field.h"

#include <boost/lexical_cast.hpp>

User::User(std::string username, std::string password)
    : m_username(username)
    , m_password(password) {}

void User::operator=(sqlite3_stmt* stmt) {
    // TODO: TEMP COMMENT UNTIL FIELD FIX
    // m_userId = sqlite3_column_blob(stmt, 0);
    Field field(m_userId);
    field = sqlite3_column_blob(stmt, 0);
    m_username = static_cast<const char*>(sqlite3_column_blob(stmt, 1));
    m_password = boost::lexical_cast<std::string>(sqlite3_column_text(stmt, 2));

    std::cout << m_userId << ' ' << m_username << ' ' << m_password << std::endl;
}

void User::buildStmt(sqlite3* db, sqlite3_stmt** stmt) {
    std::string query = "SELECT * FROM users WHERE username = ?";
    int ret = sqlite3_prepare_v2(db, query.c_str(), query.length(), stmt, 0);
    if (ret) {
        std::cerr << "Prepare failed\n";
        return;
    }

    std::cout << "Bind ret: "
              << sqlite3_bind_text(
                         *stmt, 1, m_username.c_str(), m_username.length(),
                         nullptr
                 )
              << '\n';
}