#include "Field.h"

std::ostream& operator<<(std::ostream& os, const Field& field) {
    return field.output(os);
}

std::istream& operator>>(std::istream& is, const Field& field) {
    return field.input(is);
}

Field& Field::operator=(int data) { throw; }
Field& Field::operator=(float data) { throw; }
Field& Field::operator=(bool data) { throw; }
Field& Field::operator=(const char* data) { throw; }
Field& Field::operator=(const void* data) { throw; }