#include "CommandHash.hpp"

#include <iterator>

#include <rsa_cpp/sha512.hpp>


boost::multiprecision::cpp_int hash(const Command& command)
{
    // Compute the SHA512 has of the command+unique_identifier
    const auto hasher = std::make_unique<cryptb::sha512>();
    hasher->update(reinterpret_cast<const std::uint8_t*>(command.command().c_str()), command.command().size());
    hasher->update(reinterpret_cast<const std::uint8_t*>(command.unique_identifier().c_str()), command.unique_identifier().size());
    return to_cpp_int(hasher->digest());
}

boost::multiprecision::cpp_int to_cpp_int(const std::span<const std::uint8_t> bytes)
{
    boost::multiprecision::cpp_int cpp_int;
    boost::multiprecision::import_bits(cpp_int, bytes.begin(), bytes.end(), 8, true);
    return cpp_int;
}

std::vector<std::uint8_t> to_bytes(const boost::multiprecision::cpp_int& cpp_int)
{
    std::vector<std::uint8_t> bytes;
    boost::multiprecision::export_bits(cpp_int, std::back_inserter(bytes), 8, true);
    return bytes;
}
