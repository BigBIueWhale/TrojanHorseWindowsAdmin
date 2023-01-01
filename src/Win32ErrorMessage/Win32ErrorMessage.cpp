#include "Win32ErrorMessage.hpp"
#include <error_message_macro.hpp>
#include <stdexcept>
#include <sstream>

#include <Windows.h>

Win32ErrorMessage::Win32ErrorMessage() = default;
Win32ErrorMessage::~Win32ErrorMessage() = default;
std::string Win32ErrorMessage::GetErrorMessage(std::uint32_t errorCode)
{
    LPSTR lpMsgBuf = nullptr;
    DWORD dwChars = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&lpMsgBuf,
        0, NULL);
    if (dwChars == 0)
    {
        DWORD dwError = GetLastError();
        std::stringstream ss;
        ss << "FormatMessageA failed with error " << dwError
           << " after calling GetLastError() with error " << errorCode;
        throw std::runtime_error{ ERROR_MESSAGE(ss.str()) };
    }
    std::string errorMessage(lpMsgBuf, dwChars);
    LocalFree(lpMsgBuf);
    return errorMessage;
}
