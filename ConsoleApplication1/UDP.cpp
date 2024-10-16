#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

using boost::asio::ip::udp;

class UdpServer {
public:
    UdpServer(boost::asio::io_context& io_context, short port)
        : socket_(io_context, udp::endpoint(udp::v4(), port)) {
        do_receive();
    }

private:
    void do_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(data_, 1024), sender_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {
                    // Perform data transformation here (data is in `data_`)
                    std::string response = "UDP data received and processed";
                    do_send(response, bytes_recvd);
                } else {
                    do_receive();
                }
            });
    }

    void do_send(const std::string& message, std::size_t length) {
        socket_.async_send_to(boost::asio::buffer(message), sender_endpoint_,
            [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
                do_receive();
            });
    }

    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    char data_[1024];
};

int main2() {
    boost::asio::io_context io_context;
    UdpServer server(io_context, 54321); // Port 54321

    std::vector<std::thread> thread_pool;
    for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
        thread_pool.emplace_back([&io_context]() { io_context.run(); });
    }

    for (auto& t : thread_pool) {
        t.join();
    }

    return 0;
}
