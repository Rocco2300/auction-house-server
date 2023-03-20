#pragma once

#include <iostream>

#include "FieldVector.h"

class Entity {
private:
    FieldVector* m_fields;

public:
    Entity() = delete;
    explicit Entity(FieldVector& fields);

    virtual size_t size();

    virtual Field& operator[](size_t index);
    virtual Field& operator[](const std::string& key);
};