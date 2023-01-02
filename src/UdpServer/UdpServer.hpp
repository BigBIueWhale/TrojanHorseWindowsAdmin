#pragma once

#include <csignal>
#include <cstdint>
#include <span>
#include <string_view>

// Listens on UDP localhost for commands sent in a recognized format.
// The commands are executed as administrator into the windows shell.
// The user authentication is done by a digital signature.
class UdpServer
{
public:
    virtual void Serve(volatile std::sig_atomic_t& stop, std::uint16_t port);
    virtual void OnReceivedPacket(std::span<const std::uint8_t> command) = 0;
    virtual void LogError(std::string_view sv) = 0;
    UdpServer();
    virtual ~UdpServer();
};