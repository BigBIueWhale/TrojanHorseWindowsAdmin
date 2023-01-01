#include "PathToCurrentExecutable.hpp"
#include <error_message_macro.hpp>
#include "Win32ErrorMessage.hpp"

#include <stdexcept>
#include <sstream>
#include <Windows.h>

std::wstring PathToCurrentExecutable::GetPath()
{
    // Get the path to the current executable.
    // If an INSUFFICIENT_BUFFER error happens, increase the buffer size.
    // If a different error happens, throw an exception.
    // If no error happens, return the path to the current executable.
    std::wstring path_to_current_executable{};
    DWORD dwSize = MAX_PATH;
    DWORD dwError = 0;
    while (true)
    {
        path_to_current_executable = std::wstring(dwSize, L'\0');
        dwError = GetModuleFileNameW(NULL, path_to_current_executable.data(), dwSize);
        if (dwError == 0)
        {
            dwError = GetLastError();
            if (dwError != ERROR_INSUFFICIENT_BUFFER)
            {
                std::stringstream ss;
                ss << "GetModuleFileNameW failed with error " << Win32ErrorMessage{}.GetErrorMessage(dwError);
                throw std::runtime_error{ ERROR_MESSAGE(ss.str()) };
            }
        }
        else
        {
            path_to_current_executable.resize(dwError);
            break;
        }
        dwSize *= 2;
    };
    return path_to_current_executable;
}

PathToCurrentExecutable::PathToCurrentExecutable() = default;
PathToCurrentExecutable::~PathToCurrentExecutable() = default;
