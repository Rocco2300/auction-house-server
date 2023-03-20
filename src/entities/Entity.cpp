#include "Entity.h"

Entity::Entity(FieldVector& fields)
    : m_fields(&fields) {}

size_t Entity::size() { return m_fields->size(); }

Field& Entity::operator[](size_t index) { return m_fields->at(index); }

Field& Entity::operator[](const std::string& key) { return m_fields->at(key); }