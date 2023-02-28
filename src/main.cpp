#include "Listener.h"
#include "Session.h"

#include <iostream>
#include <thread>

int main() {
    const auto address = net::ip::make_address("127.0.0.1");
    const auto port    = static_cast<unsigned short>(std::atoi("8083"));

    net::io_context ioc(4);

    std::make_shared<Listener>(ioc, tcp::endpoint{address, port})->run();

    std::vector<std::thread> v;
    v.reserve(3);
    for (auto i = 3; i > 0; i--) {
        v.emplace_back([&ioc] { ioc.run(); });
    }
    ioc.run();

    return 0;
}