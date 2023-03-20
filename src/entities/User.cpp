#include "User.h"

#include "Field.h"

#include <boost/lexical_cast.hpp>

User::User(std::string username, std::string password)
    : m_username(username)
    , m_password(password) {}


size_t User::size() { return fields.size(); }

Field& User::operator[](size_t index) {
    return fields[index];
}

Field& User::operator[](const std::string& key) { return fields[key]; }