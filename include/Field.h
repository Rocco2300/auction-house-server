#pragma once

#include <iostream>
#include <stdexcept>

#include <boost/lexical_cast.hpp>
#include <fmt/core.h>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

// Numbers to match the nlohmann impl
enum class FieldType : uint8_t {
    String   = 3,
    Integer  = 5,
    Floating = 7
};

class Field {
public:
    using Type  = FieldType;
    using Value = void*;

    friend fmt::formatter<Field>;
    friend void from_json(json& j, Field& field);
    friend void to_json(json& j, const Field& field);

private:
    std::string m_name{};

    Type  m_type{};
    Value m_value{};

public:
    Field() = default;
    Field(Type type)
        : m_type(type) {
        switch (m_type) {
        case Type::Integer:
            m_value = std::calloc(1, sizeof(int));
            break;
        case Type::Floating:
            m_value = std::calloc(1, sizeof(float));
            break;
        case Type::String:
            m_value = std::calloc(1, sizeof(std::string));
            break;
        }
    }
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


    Field& operator=(const Field& other) {
        if (m_type != other.m_type) {
            throw std::logic_error("Field types not matching.");
        }

        switch (m_type) {
        case Type::Integer:
            *static_cast<int*>(m_value) = *static_cast<int*>(other.m_value);
            break;
        case Type::Floating:
            *static_cast<float*>(m_value) = *static_cast<float*>(other.m_value);
            break;
        case Type::String:
            *static_cast<std::string*>(m_value) =
                    *static_cast<std::string*>(other.m_value);
            break;
        }

        return *this;
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
        if (static_cast<uint8_t>(value.type()) !=
            static_cast<uint8_t>(m_type)) {
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

    const std::string& name() const { return m_name; }

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

    friend bool operator==(const Field& f1, const Field& f2) {
        if (f1.m_type != f2.m_type) {
            return false;
        }

        switch (f1.m_type) {
        case Type::Integer:
            return *static_cast<int*>(f1.m_value) ==
                   *static_cast<int*>(f2.m_value);
        case Type::Floating:
            return *static_cast<float*>(f1.m_value) ==
                   *static_cast<float*>(f2.m_value);
        case Type::String:
            return *static_cast<std::string*>(f1.m_value) ==
                   *static_cast<std::string*>(f2.m_value);
        }
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

inline void from_json(json& j, Field& field) {
    switch (field.m_type) {
    case Field::Type::Integer:
        *static_cast<int*>(field.m_value) = j;
        break;
    case Field::Type::Floating:
        *static_cast<float*>(field.m_value) = j;
        break;
    case Field::Type::String:
        *static_cast<std::string*>(field.m_value) = j;
        break;
    };
}

inline void to_json(json& j, const Field& field) {
    switch (field.m_type) {
    case Field::Type::Integer:
        j = *static_cast<int*>(field.m_value);
        break;
    case Field::Type::Floating:
        j = *static_cast<float*>(field.m_value);
        break;
    case Field::Type::String:
        j = *static_cast<std::string*>(field.m_value);
        break;
    };
}