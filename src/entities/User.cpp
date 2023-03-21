#include "User.h"

#include "Field.h"

#include <boost/lexical_cast.hpp>

User::User()
    : Entity(tableName, entityName, fields) {}

User::User(std::string username, std::string password)
    : Entity(tableName, entityName, fields)
    , m_username(username)
    , m_password(password) {}

User& User::operator=(const json& other) {
    return dynamic_cast<User&>(Entity::operator=(other));
}