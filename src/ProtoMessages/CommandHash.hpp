#pragma once

#include "messages.pb.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <span>

// It doesn't really matter how the hash function works, as long as it's cryptograhpic,
// and both the client and the server use this function.
extern boost::multiprecision::cpp_int hash(const Command& command);

extern boost::multiprecision::cpp_int to_cpp_int(const std::span<const std::uint8_t> bytes);

extern std::vector<std::uint8_t> to_bytes(const boost::multiprecision::cpp_int& cpp_int);
