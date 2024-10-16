#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/array.hpp>
#include <vector>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

// Define a thread pool for handling client connections
boost::thread_group threadpool;
boost::asio::io_context io_context;

// TCP Server
class tcp_connection {
public:
    tcp_connection(boost::asio::io_context& io_context) : socket_(io_context) {}

    tcp::socket& socket() { return socket_; }

    void start() {
        boost::array<char, 128> buf;
        socket_.async_read_some(boost::asio::buffer(buf),
            boost::bind(&tcp_connection::handle_read, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

private:
    void handle_read(const boost::system::error_code& error,
        size_t bytes_transferred) {
        if (!error) {
            // Process data (example transformation)
            std::string received_data(buf_.data(), bytes_transferred);
            std::string processed_data = ""Processed: "" + received_data;

            boost::asio::async_write(socket_, boost::asio::buffer(processed_data),
                boost::bind(&tcp_connection::handle_write, shared_from_this(),
                    boost::asio::placeholders::error));
        }
        else {
            // Handle error (e.g., client disconnect)
            std::cerr << ""TCP Error : "" << error.message() << std::endl;
        }
    }

    void handle_write(const boost::system::error_code& error) {
        if (error) {
            std::cerr << ""TCP Error : "" << error.message() << std::endl;
        }
    }

    tcp::socket socket_;
    boost::array<char, 128> buf_;
};

void start_tcp_server(int port) {
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
    while (true) {
        tcp_connection::pointer new_connection(new tcp_connection(io_context));
        acceptor.accept(new_connection->socket());
        threadpool.create_thread(boost::bind(&tcp_connection::start, new_connection));
    }
}


// UDP Server
void start_udp_server(int port) {
    udp::socket socket(io_context, udp::endpoint(udp::v4(), port));
    boost::array<char, 1024> recv_buffer;
    udp::endpoint remote_endpoint;

    while (true) {
        size_t bytes_received = socket.receive_from(
            boost::asio::buffer(recv_buffer), remote_endpoint);

        // Process data (example transformation)
        std::string received_data(recv_buffer.data(), bytes_received);
        std::string processed_data = ""Processed UDP : "" + received_data;

        socket.send_to(boost::asio::buffer(processed_data), remote_endpoint);
    }
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << ""Usage: server <tcp_port> < udp_port>"" << std::endl;
        return 1;
    }

    int tcp_port = std::atoi(argv[1]);
    int udp_port = std::atoi(argv[2]);

    // Create thread pool (adjust number of threads as needed)
    for (int i = 0; i < 4; ++i) {
        threadpool.create_thread(boost::bind(&boost::asio::io_context::run, &io_context));
    }

    // Start TCP and UDP servers in separate threads
    boost::thread tcp_server_thread(boost::bind(&start_tcp_server, tcp_port));
    boost::thread udp_server_thread(boost::bind(&start_udp_server, udp_port));

    tcp_server_thread.join(); // These joins will block indefinitely
    udp_server_thread.join();

    return 0;
}