#include "Field.h"

#include <stdexcept>

std::ostream& operator<<(std::ostream& os, const Field& field) {
    return field.output(os);
}

std::istream& operator>>(std::istream& is, const Field& field) {
    return field.input(is);
}

Field& Field::operator=(int data) {
    throw std::logic_error("Unimplemented int assigment operator");
}
Field& Field::operator=(float data) {
    throw std::logic_error("Unimplemented float assigment operator");
}
Field& Field::operator=(bool data) {
    throw std::logic_error("Unimplemented bool assigment operator");
}
Field& Field::operator=(const char* data) {
    throw std::logic_error("Unimplemented string assigment operator");
}
Field& Field::operator=(const void* data) {
    throw std::logic_error("Unimplemented void assigment operator");
}