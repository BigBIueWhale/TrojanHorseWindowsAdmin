#include "UdpServer.hpp"

#include <error_message_macro.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <chrono>
#include <vector>
#include <memory>

void UdpServer::Serve(volatile std::sig_atomic_t& stop, std::uint16_t port)
{
    const auto butler = std::make_shared<boost::asio::io_context>();
    // Create a UDP socket to listen on localhost
    boost::asio::ip::udp::socket socket{ *butler, boost::asio::ip::udp::endpoint{ boost::asio::ip::address::from_string("localhost"), port } };
    
    // Max UDP packet length
    const auto buffer = std::make_shared<std::vector<std::uint8_t>>(65507, 0);

    socket.async_receive(boost::asio::buffer(*buffer), [this, buffer](const boost::system::error_code& error, std::size_t bytes_received)
    {
        if (!error)
        {
            this->OnReceivedPacket(std::span<const std::uint8_t>{ buffer->data(), bytes_received });
        }
        else
        {
            this->LogError(ERROR_MESSAGE(error.message()));
        }
    });

    while (!stop)
    {
        // Minimum sleep time in Windows is 26 milliseconds
        butler->run_for(std::chrono::milliseconds{ 14 });
    }
    // Cancel socket async_receive
    socket.cancel();
}

UdpServer::UdpServer() = default;

UdpServer::~UdpServer() = default;
