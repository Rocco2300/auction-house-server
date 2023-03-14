#pragma once

#include "Field.h"

class StringField : public Field {
public:
    StringField() = default;
    explicit StringField(const char* data);
    explicit StringField(const void* data);

    StringField& operator=(const StringField& field);

    StringField& operator=(const char* data) override;
    StringField& operator=(const void* data) override;

    ~StringField() override = default;

    std::ostream& output(std::ostream& os) override;
    std::istream& input(std::istream& is) override;

private:
    void alloc(const char* data);
    void realloc(const char* data);
};