#include "Entity.h"

#include <fmt/core.h>

Entity::Entity(
        std::string& tableName, std::string& entityName, FieldVector& fields
)
    : m_tableName(&tableName)
    , m_entityName(&entityName)
    , m_fields(&fields) {}

Entity& Entity::operator=(const json& other) {
    json temp = other.at(*m_entityName);

    for (const auto& x : temp.items()) {
        m_fields->at(x.key()) = x.value();
    }

    return *this;
}

Entity& Entity::operator=(const Entity& other) {
    if (*m_entityName != *other.m_entityName) {
        throw std::logic_error(fmt::format(
                "cannot assign {} to {}", *other.m_entityName, *m_entityName
        ));
    }

    for (size_t i = 0; i < size(); i++) {
        m_fields->at(i) = other.m_fields->at(i);
    }

    return *this;
}

size_t Entity::size() const { return m_fields->size(); }

std::string Entity::name() const { return *m_entityName; }

std::string Entity::tableName() const { return *m_tableName; }

Field& Entity::operator[](size_t index) { return m_fields->at(index); }

Field& Entity::operator[](const std::string& key) { return m_fields->at(key); }

const Field& Entity::operator[](size_t index) const {
    return m_fields->at(index);
}

const Field& Entity::operator[](const std::string& key) const {
    return m_fields->at(key);
}

Field& Entity::at(size_t index) { return m_fields->at(index); }

Field& Entity::at(const std::string& key) { return m_fields->at(key); }

const Field& Entity::at(size_t index) const { return m_fields->at(index); }

const Field& Entity::at(const std::string& key) const {
    return m_fields->at(key);
}