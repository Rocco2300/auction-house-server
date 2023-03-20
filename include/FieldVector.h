#pragma once

#include "Field.h"

#include <initializer_list>
#include <utility>
#include <vector>

class FieldVector {
private:
    using Iterator = std::vector<Field>::iterator;

private:
    std::vector<Field> m_fields;

public:
    FieldVector() = default;
    FieldVector(std::initializer_list<Field> fields) {
        m_fields = std::vector<Field>(fields);
    }

    ~FieldVector() = default;

    size_t size() { return m_fields.size(); }

    Field& operator[](size_t index) { return m_fields.at(index); }

    const Field& operator[](size_t index) const { return m_fields.at(index); }

    Field& operator[](std::string key) {
        for (auto& elem : m_fields) {
            if (elem.name() == key) {
                return elem;
            }
        }
        throw std::out_of_range("Key doesn't exist in FieldVector.");
    }

    const Field& operator[](std::string key) const {
        for (auto& elem : m_fields) {
            if (elem.name() == key) {
                return elem;
            }
        }
        throw std::out_of_range("Key doesn't exist in FieldVector.");
    }

    Field& at(size_t index) { return m_fields.at(index); }

    const Field& at(size_t index) const { return m_fields.at(index); }

    Field& at(std::string key) {
        for (auto& elem : m_fields) {
            if (elem.name() == key) {
                return elem;
            }
        }
        throw std::out_of_range("Key doesn't exist in FieldVector.");
    }

    const Field& at(std::string key) const {
        for (auto& elem : m_fields) {
            if (elem.name() == key) {
                return elem;
            }
        }
        throw std::out_of_range("Key doesn't exist in FieldVector.");
    }


    Iterator begin() {
        return m_fields.begin();
    }

    Iterator end() {
        return m_fields.end();
    }
};