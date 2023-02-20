#include "Session.h"
#include "Global.h"
#include "Logger.h"

#include <boost/asio/dispatch.hpp>

Session::Session(tcp::socket&& socket)
    : m_websocket(std::move(socket)) {}

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

    doWrite();
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

    global::requests.push_back(
            {shared_from_this(), beast::buffers_to_string(m_buffer.data())}
    );
    m_buffer.consume(m_buffer.size());

    doRead();
}
void Session::doWrite() {
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
}

void Session::onWrite(beast::error_code ec, std::size_t bytesTransferred) {
    boost::ignore_unused(bytesTransferred);

    if (ec) {
        logFail(ec, "Write");
        return;
    }

    //    m_buffer.consume(m_buffer.size());

    doRead();
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