#pragma once

#include <cstdint>
#include <string>

class Win32ErrorMessage
{
public:
    virtual std::string GetErrorMessage(std::uint32_t errorCode);

    // Make future-proof by avoiding assumptions that the user might make
    Win32ErrorMessage();
    virtual ~Win32ErrorMessage();
    Win32ErrorMessage(const Win32ErrorMessage&) = delete;
    Win32ErrorMessage& operator=(const Win32ErrorMessage&) = delete;
    Win32ErrorMessage(Win32ErrorMessage&&) = delete;
    Win32ErrorMessage& operator=(Win32ErrorMessage&&) = delete;
};
