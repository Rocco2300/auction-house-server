#include "Session.h"
#include "Logger.h"

#include <boost/asio/dispatch.hpp>

using namespace std::placeholders;

Session::Session(tcp::socket&& socket, MessageHandler& messageHandler)
    : m_websocket(std::move(socket))
    , m_messageHandler(messageHandler) {}

void Session::run() {
    net::dispatch(
            m_websocket.get_executor(),
            beast::bind_front_handler(&Session::onRun, shared_from_this())
    );
}
void Session::onRun() {
    m_websocket.set_option(
            websocket::stream_base::timeout::suggested(beast::role_type::server)
    );

    m_websocket.async_accept(
            beast::bind_front_handler(&Session::onAccept, shared_from_this())
    );
}

void Session::onAccept(beast::error_code ec) {
    if (ec) {
        logFail(ec, "Accept");
        return;
    }

    auto self = shared_from_this();
    m_websocket.async_write(
            net::buffer("Welcome user!"),
            [self](beast::error_code ec, std::size_t bytesTransferred) {
                if (ec) {
                    std::cout << "Error in sending welcome message!\n"
                              << std::flush;
                }
            }
    );

    doRead();
}

void Session::doRead() {
    m_websocket.async_read(
            m_buffer,
            beast::bind_front_handler(&Session::onRead, shared_from_this())
    );
}

void Session::onRead(beast::error_code ec, std::size_t bytesTransferred) {
    boost::ignore_unused(bytesTransferred);

    if (ec == websocket::error::closed) {
        return;
    }

    if (ec) {
        logFail(ec, "Read");
        return;
    }

    m_websocket.text(m_websocket.got_text());

    m_messageHandler.enqueueRequest(
            beast::buffers_to_string(m_buffer.data()),
            std::bind(&Session::doWrite, shared_from_this(), _1)
    );

    m_buffer.consume(m_buffer.size());

    doRead();
}

void Session::doWrite(Message message) {
    auto self = shared_from_this();

    m_websocket.async_write(
            net::buffer(message),
            beast::bind_front_handler(&Session::onWrite, shared_from_this())
    );
}

void Session::onWrite(beast::error_code ec, std::size_t bytesTransferred) {
    boost::ignore_unused(bytesTransferred);

    if (ec) {
        logFail(ec, "Write");
        return;
    }
}

void Session::sendMessage(const std::string& message) {
    auto self = shared_from_this();

    m_websocket.async_write(
            net::buffer(message),
            [self](beast::error_code ec, std::size_t bytesTransferred) {
                if (ec) {
                    std::cout << "Error in sending message!\n" << std::flush;
                }
            }
    );
}