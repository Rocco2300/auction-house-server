#include <gtest/gtest.h>

#include "Field.h"

TEST(field, invalid_assign) {
    int         intRef    = 2;
    float       floatRef  = 3.2f;
    std::string stringRef = "heya";

    Field intField(intRef);
    Field floatField(floatRef);
    Field stringField(stringRef);

    EXPECT_THROW(intField = floatRef, std::exception);
    EXPECT_THROW(intField = stringRef, std::exception);

    EXPECT_THROW(floatField = intRef, std::exception);
    EXPECT_THROW(floatField = stringRef, std::exception);

    EXPECT_THROW(stringField = intRef, std::exception);
    EXPECT_THROW(stringField = floatRef, std::exception);
}

TEST(field, int_cast) {
    int   intRef = 2;
    Field intField(intRef);

    int         castInt;
    float       castFloat;
    std::string castString;

    EXPECT_NO_THROW(static_cast<int>(intField));

    EXPECT_NO_THROW(castInt = intField);
    EXPECT_NO_THROW(castFloat = intField);
    EXPECT_NO_THROW(castString = intField);
}