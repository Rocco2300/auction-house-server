#include "User.h"

#include "Field.h"

#include <boost/lexical_cast.hpp>

User::User() : Entity(fields) { }

User::User(std::string username, std::string password)
    : Entity(fields)
    , m_username(username)
    , m_password(password) {}