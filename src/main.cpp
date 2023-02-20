#include "Listener.h"
#include "Session.h"
#include "Global.h"

#include <iostream>
#include <thread>

void pollInput()
{
    while (1)
    {
        while (!global::requests.empty())
        {
            auto& currRequest = global::requests.back();

            std::string response;
            response += "response: ";
            response += currRequest.getData();

            currRequest.setResponse(response);
            currRequest.sendResponse();

            global::requests.pop_back();
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