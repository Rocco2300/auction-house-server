#pragma once

#include <istream>
#include <ostream>

class Field {
protected:
    void* m_data{};

public:
    virtual Field& operator=(int data);
    virtual Field& operator=(float data);
    virtual Field& operator=(bool data);
    virtual Field& operator=(const char* data);
    virtual Field& operator=(const void* data);

    virtual ~Field();

    friend std::ostream& operator<<(std::ostream& os, Field& field);
    friend std::istream& operator>>(std::istream& is, Field& field);

protected:
    virtual std::ostream& output(std::ostream& os) = 0;
    virtual std::istream& input(std::istream& is)  = 0;
};