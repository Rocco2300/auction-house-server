#include <gtest/gtest.h>

#include "IntField.h"

TEST(int_field, nulltest) {
    Field*      i   = new IntField();
    const void* ptr = nullptr;

    *i = ptr;
}