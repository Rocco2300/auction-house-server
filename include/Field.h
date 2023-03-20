#pragma once

#include <iostream>
#include <stdexcept>

#include <boost/lexical_cast.hpp>
#include <fmt/core.h>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

class Field {
private:
    // Numbers to match the nlohmann impl
    enum class ValueType : uint8_t {
        String   = 3,
        Integer  = 5,
        Floating = 7
    };

    friend bool operator==(json::value_t valueType1, ValueType valueType2) {
        return static_cast<uint8_t>(valueType1) ==
               static_cast<uint8_t>(valueType2);
    }

    friend bool operator==(ValueType valueType1, json::value_t valueType2) {
        return static_cast<uint8_t>(valueType1) ==
               static_cast<uint8_t>(valueType2);
    }

public:
    using Type  = ValueType;
    using Value = void* const;

    friend fmt::formatter<Field>;

private:
    std::string m_name;

    Type  m_type{};
    Value m_value{};

public:
    Field() = default;
    Field(const std::string& fieldName, int& value)
        : m_name(fieldName)
        , m_type(Type::Integer)
        , m_value(&value) {}
    Field(const std::string& fieldName, float& value)
        : m_name(fieldName)
        , m_type(Type::Floating)
        , m_value(&value) {}
    Field(const std::string& fieldName, std::string& value)
        : m_name(fieldName)
        , m_type(Type::String)
        , m_value(&value) {}

    const std::string& name() {
        return m_name;
    }

    // This constructor is used for db interface
    // and atm sqlite3 returns blob as a char array
    // so we first convert to char and then work with that
    Field& operator=(const void* value) {
        using boost::lexical_cast;

        if (!value) {
            return *this;
        }

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

    Field& operator=(json value) {
        if (value.type() != m_type) {
            return *this;
        }

        switch (m_type) {
        case Type::Integer:
            *static_cast<int*>(m_value) = value;
            break;
        case Type::Floating:
            *static_cast<float*>(m_value) = value;
            break;
        case Type::String:
            *static_cast<std::string*>(m_value) = value;
            break;
        }

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

    operator json() {
        int         integer;
        float       floating;
        std::string string;

        switch (m_type) {
        case Type::Integer:
            integer = *static_cast<int*>(m_value);
            return static_cast<json>(integer);
        case Type::Floating:
            floating = *static_cast<float*>(m_value);
            return static_cast<json>(floating);
        case Type::String:
            string = *static_cast<std::string*>(m_value);
            return static_cast<json>(string);
        }

        return static_cast<json>(nullptr);
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

template<>
struct fmt::formatter<Field>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const Field& field, FormatContext& ctx) {
        if (!field.m_value) {
            throw std::logic_error("Cannot format NULL m_value*");
        }

        switch (field.m_type) {
        case Field::Type::Integer:
            return fmt::format_to(
                    ctx.out(), "{}", *static_cast<int*>(field.m_value)
            );
        case Field::Type::Floating:
            return fmt::format_to(
                    ctx.out(), "{}", *static_cast<float*>(field.m_value)
            );
        case Field::Type::String:
            return fmt::format_to(
                    ctx.out(), "{}", *static_cast<std::string*>(field.m_value)
            );
        }

        throw std::logic_error("Unhandled type case in format");
    }
};