#pragma once

#include "MessageHandler.h"

#include <memory>

#include <boost/beast/core.hpp>

namespace beast     = boost::beast;
namespace websocket = beast::websocket;
namespace net       = boost::asio;
using tcp           = boost::asio::ip::tcp;

class Listener : public std::enable_shared_from_this<Listener> {
private:
    net::io_context& m_ioc;
    tcp::acceptor    m_acceptor;

    MessageHandler m_messageHandler;

public:
    Listener(net::io_context& ioc, tcp::endpoint endpoint);

    void run();

private:
    void doAccept();
    void onAccept(beast::error_code ec, tcp::socket socket);
};