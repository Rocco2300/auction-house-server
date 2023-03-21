#include "Entity.h"

Entity::Entity(
        std::string& tableName, std::string& entityName, FieldVector& fields
)
    : m_tableName(&tableName)
    , m_entityName(&entityName)
    , m_fields(&fields) {}

Entity& Entity::operator=(const json& other) {
    std::cout << "in entity\n";

    json temp = other.at(*m_entityName);

    for (const auto& x : temp.items()) {
        m_fields->at(x.key()) = x.value();
    }

    return *this;
}

size_t Entity::size() const { return m_fields->size(); }

std::string Entity::name() const { return *m_entityName; }

Field& Entity::operator[](size_t index) { return m_fields->at(index); }

Field& Entity::operator[](const std::string& key) { return m_fields->at(key); }

const Field& Entity::operator[](size_t index) const {
    return m_fields->at(index);
}

const Field& Entity::operator[](const std::string& key) const {
    return m_fields->at(key);
}