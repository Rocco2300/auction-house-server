#include "StringField.h"

#include <cstring>

StringField::StringField(const char* data) { alloc(data); }

StringField::StringField(const void* data) {
    auto buf = static_cast<const char*>(data);
    alloc(buf);
}

StringField& StringField::operator=(const StringField& field) {
    if (m_data) {
        std::free(m_data);
    }

    auto buf = static_cast<const char*>(field.m_data);
    alloc(buf);
    return *this;
}

StringField& StringField::operator=(const char* data) {
    if (!m_data) {
        alloc(data);
        return *this;
    }

    auto size     = std::strlen(data);
    auto currSize = std::strlen(static_cast<char*>(m_data));
    if (size > currSize) {
        realloc(data);
        return *this;
    }

    std::memcpy(m_data, data, size);
    return *this;
}

StringField& StringField::operator=(const void* data) {}

std::ostream& StringField::output(std::ostream& os) {
    os << static_cast<char*>(m_data);
    return os;
}

std::istream& StringField::input(std::istream& is) {
    char* data = new char[1000];
    std::memset(data, 0, 1000);
    is.getline(data, 1000);

    if (!m_data) {
        alloc(data);
        return is;
    }

    auto size     = std::strlen(data);
    auto currSize = std::strlen(static_cast<char*>(m_data));
    if (size > currSize) {
        realloc(data);
        return is;
    }

    std::memcpy(m_data, data, size);
    return is;
}

void StringField::alloc(const char* data) {
    auto size = std::strlen(data);
    m_data    = std::malloc(size * sizeof(char));
    std::memcpy(m_data, data, size);
}

void StringField::realloc(const char* data) {
    std::free(m_data);
    alloc(data);
}