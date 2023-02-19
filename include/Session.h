#pragma once

#include <memory>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

namespace beast     = boost::beast;
namespace websocket = beast::websocket;
namespace net       = boost::asio;
using tcp           = boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
private:
    websocket::stream<beast::tcp_stream> m_websocket;
    beast::flat_buffer m_buffer;

public:
    explicit Session(tcp::socket&& socket);

    void run();
    void onRun();
    void onAccept(beast::error_code ec);

    void doRead();
    void onRead(beast::error_code ec, std::size_t bytesTransferred);
    void doWrite();
    void onWrite(beast::error_code ec, std::size_t bytesTransferred);

    void sendMessage(const std::string& message);
};