#include "Session.h"
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
    m_websocket.async_write(
            m_buffer.data(),
            beast::bind_front_handler(&Session::onWrite, shared_from_this())
    );
}
void Session::doWrite() {}

void Session::onWrite(beast::error_code ec, std::size_t bytesTransferred) {
    boost::ignore_unused(bytesTransferred);

    if (ec) {
        logFail(ec, "Write");
        return;
    }

    m_buffer.consume(m_buffer.size());

    doRead();
}