#pragma once

#include "PublicPrivateKeyTypes.hpp"
#include <cstdint>

extern const PublicKey_t g_public_key;

// Choose any non-reserved port number, definitely above 1023
static constexpr std::uint16_t g_port_number{ 19284 };
