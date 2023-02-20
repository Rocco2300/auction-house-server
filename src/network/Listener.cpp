#include "Listener.h"
#include "Logger.h"
#include "Session.h"

#include <iostream>
#include <string>

#include <boost/asio/strand.hpp>

Listener::Listener(net::io_context& ioc, tcp::endpoint endpoint)
    : m_ioc(ioc)
    , m_acceptor(ioc) {
    beast::error_code ec;

    m_acceptor.open(endpoint.protocol(), ec);
    if (ec) {
        logFail(ec, "Open");
        return;
    }

    m_acceptor.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
        logFail(ec, "SetOpt");
        return;
    }

    m_acceptor.bind(endpoint, ec);
    if (ec) {
        logFail(ec, "Bind");
        return;
    }

    m_acceptor.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
        logFail(ec, "Listen");
        return;
    }
}

void Listener::run() { doAccept(); }

void Listener::doAccept() {
    m_acceptor.async_accept(
            net::make_strand(m_ioc),
            beast::bind_front_handler(&Listener::onAccept, shared_from_this())
    );
}

void Listener::onAccept(beast::error_code ec, tcp::socket socket) {
    if (ec) {
        logFail(ec, "Accept");
    } else {
        std::make_shared<Session>(std::move(socket))->run();
    }

    doAccept();
}