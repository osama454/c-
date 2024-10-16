#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

using boost::asio::ip::tcp;

class TcpSession : public std::enable_shared_from_this<TcpSession> {
public:
    TcpSession(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    // Perform data transformation here (data is in `data_`)
                    // Simulating data transformation
                    std::string response = "Data received and transformed";
                    do_write(response);
                }
            });
    }

    void do_write(std::string response) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(response),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    do_read();
                }
            });
    }

    tcp::socket socket_;
    char data_[1024];
};

class TcpServer {
public:
    TcpServer(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<TcpSession>(std::move(socket))->start();
                }
                do_accept();
            });
    }

    tcp::acceptor acceptor_;
};

int main() {
    boost::asio::io_context io_context;
    TcpServer server(io_context, 12345); // Port 12345

    std::vector<std::thread> thread_pool;
    for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
        thread_pool.emplace_back([&io_context]() { io_context.run(); });
    }

    for (auto& t : thread_pool) {
        t.join();
    }

    return 0;
}
