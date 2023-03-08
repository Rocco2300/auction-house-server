#include "Session.h"
#include "Logger.h"

#include <boost/algorithm/string.hpp>
#include <boost/asio/dispatch.hpp>

using namespace std::placeholders;

Session::Session(
        tcp::socket&& socket, MessageHandler& messageHandler,
        SessionManager& sessionManager
)
    : m_websocket(std::move(socket))
    , m_messageHandler(messageHandler)
    , m_sessionManager(sessionManager) {}

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
        m_sessionManager.disconnectSession(m_username);
        return;
    }

    if (ec) {
        logFail(ec, "Read");
        return;
    }

    m_websocket.text(m_websocket.got_text());

    auto message = beast::buffers_to_string(m_buffer.data());
    if (message.find("username") != std::string::npos) {
        auto usernamePos =
                message.find("username") + std::string("username").length() + 1;
        auto ending = message.find(";");

        if (m_username.length() == 0) {
            m_username = message.substr(usernamePos, ending - 1);
            boost::trim(m_username);
            m_sessionManager.registerSession(m_username, shared_from_this());
        }
    }

    m_messageHandler.enqueueRequest(
            message, [capture0 = shared_from_this()](auto&& PH1
                     ) { capture0->doWrite(std::forward<decltype(PH1)>(PH1)); }
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