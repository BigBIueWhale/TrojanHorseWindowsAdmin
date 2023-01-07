#include "HiddenFileCreate.hpp"

#include <Win32ErrorMessage.hpp>
#include <error_message_macro.hpp>
#include <sstream>
#include <stdexcept>

#include <Windows.h>


HiddenFileCreate::HiddenFileCreate() = default;
HiddenFileCreate::~HiddenFileCreate() = default;

void HiddenFileCreate::Create(const std::wstring_view path)
{
    // Create the file if it doesn't exist, do nothing if it does
    const HANDLE file_handle = CreateFileW(path.data(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_HIDDEN, nullptr);
    if (file_handle == INVALID_HANDLE_VALUE)
    {
        const DWORD error_code = GetLastError();
        if (error_code == ERROR_FILE_EXISTS)
            return;
        const std::string error_message = Win32ErrorMessage{}.GetErrorMessage(error_code);
        std::ostringstream err_msg;
        err_msg << "Failed to create file. Error: " << error_code << " and error message " << error_message;
        throw std::runtime_error{ ERROR_MESSAGE(err_msg.str()) };
    }
    CloseHandle(file_handle);
}
