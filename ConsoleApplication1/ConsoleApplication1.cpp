#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;

class TCPServer {
public:
    TCPServer(boost::asio::io_service& io_service, short port)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
    }

private:
    void start_accept() {
        TCPConnection::pointer new_connection = TCPConnection::create(acceptor_.get_io_service());
        acceptor_.async_accept(new_connection->socket(),
            boost::bind(&TCPServer::handle_accept, this, new_connection,
                boost::asio::placeholders::error));
    }

    void handle_accept(TCPConnection::pointer new_connection, const boost::system::error_code& error) {
        if (!error) {
            new_connection->start();
        }
        start_accept();
    }

    tcp::acceptor acceptor_;
};

int main2() {
    try {
        boost::asio::io_service io_service;
        TCPServer server(io_service, 12345);
        io_service.run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
