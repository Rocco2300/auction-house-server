#pragma once

#include "Field.h"

#include <cstring>
#include <memory>

class IntField : public Field {
public:
    IntField();
    explicit IntField(int data);
    explicit IntField(const void* data);

    IntField& operator=(int data) override;
    IntField& operator=(const void* data) override;

    ~IntField() override = default;

    std::ostream& output(std::ostream& os) override;
    std::istream& input(std::istream& is) override;
};