#include "Listener.h"
#include "Session.h"
#include "Global.h"

#include <iostream>
#include <thread>

void pollInput()
{
    while (1)
    {
        std::string message;
        std::cout << "Message: ";
        std::cin  >> message;

        for (auto i = 0; i < global::sessions.size(); i++)
        {
            if (global::sessions[i].expired())
                continue;

            global::sessions[i].lock()->sendMessage(message);
        }
    }
}

int main() {
    const auto address = net::ip::make_address("127.0.0.1");
    const auto port    = static_cast<unsigned short>(std::atoi("8083"));

    net::io_context ioc(4);

    std::make_shared<Listener>(ioc, tcp::endpoint{address, port})->run();

    std::thread ioThread(pollInput);
    std::vector<std::thread> v;
    v.reserve(3);
    for (auto i = 3; i > 0; i--) {
        v.emplace_back([&ioc] { ioc.run(); });
    }
    ioc.run();
    ioThread.join();

    return 0;
}