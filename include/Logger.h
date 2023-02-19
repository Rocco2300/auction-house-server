#pragma once

#include <iostream>
#include <string>

#include <boost/beast/core.hpp>

namespace beast = boost::beast;

inline void logFail(beast::error_code ec, const std::string& what) {
    std::cerr << "Open: " << ec.message() << std::endl;
}