#pragma once

#include <iostream>
#include <stdexcept>

#include <boost/lexical_cast.hpp>

class Field {
private:
    enum class ValueType {
        String,
        Integer,
        Floating
    };

public:
    using Type  = ValueType;
    using Value = void* const;

private:
    Type  m_type{};
    Value m_value{};

public:
    Field() = default;
    Field(int& value)
        : m_type(Type::Integer)
        , m_value(&value) {}
    Field(float& value)
        : m_type(Type::Floating)
        , m_value(&value) {}
    Field(std::string& value)
        : m_type(Type::String)
        , m_value(&value) {}

    // This constructor is used for db interface
    // and atm sqlite3 returns blob as a char array
    // so we first convert to char and then work with that
    Field& operator=(const void* value) {
        using boost::lexical_cast;

        auto buffer = static_cast<const char*>(value);

        switch (m_type) {
        case Type::Integer:
            *static_cast<int* const>(m_value) = lexical_cast<int>(buffer);
            break;
        case Type::Floating:
            *static_cast<float* const>(m_value) = lexical_cast<float>(buffer);
            break;
        case Type::String:
            *static_cast<std::string* const>(m_value) = buffer;
            break;
        }

        return *this;
    }

    Field& operator=(int value) {
        if (!isInteger()) {
            throw std::invalid_argument("Cannot assign int to another type");
        }

        *static_cast<int* const>(m_value) = value;
        return *this;
    }

    Field& operator=(float value) {
        if (!isFloating()) {
            throw std::invalid_argument("Cannot assign float to another type");
        }

        *static_cast<float* const>(m_value) = value;
        return *this;
    }

    Field& operator=(std::string value) {
        if (!isString()) {
            throw std::invalid_argument("Cannot assign string to another type");
        }

        *static_cast<std::string* const>(m_value) = value;
        return *this;
    }

    Field& operator=(const char* value) {
        if (!isString()) {
            throw std::invalid_argument("Cannot assign string to another type");
        }

        *static_cast<std::string* const>(m_value) = value;
        return *this;
    }

    operator int() {
        if (!isNumeric()) {
            throw std::bad_cast();
        }

        if (isFloating()) {
            float val = *static_cast<float*>(m_value);
            return static_cast<int>(val);
        }

        return *static_cast<int*>(m_value);
    }

    operator std::string() {
        if (!isString()) {
            throw std::bad_cast();
        }

        return *static_cast<std::string*>(m_value);
    }

    friend std::ostream& operator<<(std::ostream& os, const Field& field) {
        switch (field.m_type) {
        case Type::Integer:
            os << *static_cast<int*>(field.m_value);
            break;
        case Type::Floating:
            os << *static_cast<float*>(field.m_value);
            break;
        case Type::String:
            os << *static_cast<std::string*>(field.m_value);
            break;
        };

        return os;
    }

private:
    bool isInteger() { return m_type == Type::Integer; }

    bool isFloating() { return m_type == Type::Floating; }

    bool isNumeric() { return isInteger() || isFloating(); }

    bool isString() { return m_type == Type::String; }
};