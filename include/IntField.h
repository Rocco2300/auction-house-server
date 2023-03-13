#pragma once

#include "Field.h"

#include <cstring>
#include <memory>

class IntField : public Field {
public:
    IntField();
    explicit IntField(const void* data);

    IntField& operator=(int data) override;
    IntField& operator=(const void* data) override;

    ~IntField() override;

    std::ostream& output(std::ostream& os) const override;
    std::istream& input(std::istream& is) const override;
};