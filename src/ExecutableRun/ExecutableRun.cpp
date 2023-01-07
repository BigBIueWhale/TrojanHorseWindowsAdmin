#include "ExecutableRun.hpp"

#include <error_message_macro.hpp>
#include <sstream>
#include <stdexcept>
#include <Win32ErrorMessage.hpp>

#include <Windows.h>

void ExecutableRun::Run(const std::wstring_view path_to_executable)
{
    // Use CreateProcessW to run the executable
    STARTUPINFOW startup_info{};
    startup_info.cb = sizeof(startup_info);
    PROCESS_INFORMATION process_info{};
    const BOOL success = CreateProcessW(std::wstring{ path_to_executable }.c_str(), nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup_info, &process_info);
    if (!success)
    {
        const DWORD error_code = GetLastError();
        const std::string error_message = Win32ErrorMessage{}.GetErrorMessage(error_code);
        std::ostringstream err_msg;
        err_msg << "Failed to run executable. Error: " << error_code << " and error message " << error_message;
        throw std::runtime_error{ ERROR_MESSAGE(err_msg.str()) };
    }
}

ExecutableRun::ExecutableRun() = default;
ExecutableRun::~ExecutableRun() = default;
