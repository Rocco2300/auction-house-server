#pragma once

#include <iostream>

#include "FieldVector.h"

class Entity {
private:
    std::string* m_tableName;
    std::string* m_entityName;

    FieldVector* m_fields;

public:
    Entity() = delete;
    explicit Entity(
            std::string& tableName, std::string& entityName, FieldVector& fields
    );

    // We use this for from_json since
    // we cannot have default constructions
    // since we always need to declare fields
    // tableName and entityName
    Entity& operator=(const json& other);
    Entity& operator=(const Entity& other);

    virtual size_t size() const;
    virtual std::string name() const;
    virtual std::string tableName() const;

    virtual Field& operator[](size_t index);
    virtual Field& operator[](const std::string& key);

    virtual const Field& operator[](size_t index) const;
    virtual const Field& operator[](const std::string& key) const;

    virtual Field& at(size_t index);
    virtual Field& at(const std::string& key);

    virtual const Field& at(size_t index) const;
    virtual const Field& at(const std::string& key) const;
};

inline void to_json(json& j, const Entity& entity) {
    json temp;

    for (size_t i = 0; i < entity.size(); i++) {
        auto& field        = entity[i];
        temp[field.name()] = field;
    }

    j[entity.name()] = temp;
}