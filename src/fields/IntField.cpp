#include "IntField.h"

#include <boost/lexical_cast.hpp>

IntField::IntField() { m_data = std::malloc(sizeof(int)); }

IntField::IntField(int data) {
    m_data = std::malloc(sizeof(int));
    *static_cast<int*>(m_data) = data;
}

IntField::IntField(const void* data) {
    m_data = std::malloc(sizeof(int));
    std::memcpy(m_data, data, sizeof(int));
}

IntField& IntField::operator=(int data) {
    if (!m_data) {
        m_data = std::malloc(sizeof(int));
    }

    *static_cast<int*>(m_data) = data;
    return *this;
}

IntField& IntField::operator=(const void* data) {
    if (!m_data) {
        m_data = std::malloc(sizeof(int));
    }

    // This is done because sqlite_blob returns int as string :/ will have to change
    auto stringData = static_cast<const char*>(data);
    auto intData    = boost::lexical_cast<int>(stringData);

    std::memcpy(m_data, &intData, sizeof(int));
    return *this;
}

std::ostream& IntField::output(std::ostream& os) {
    os << *static_cast<int*>(m_data);
    return os;
}

std::istream& IntField::input(std::istream& is) {
    is >> *static_cast<int*>(m_data);
    return is;
}