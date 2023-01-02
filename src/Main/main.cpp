#include <error_message_macro.hpp>
#include <AdminCheck.hpp>
#include <PathToCurrentExecutable.hpp>
#include <AdminStartupItem.hpp>
#include <iostream>
#include <ios>
#include <iomanip>
#include <CombaseLibrary.hpp>

#include <stdexcept>
#include <sstream>
#include <type_traits>

int main()
{
    try
    {
        if (!AdminCheck{}.IsAdmin())
        {
            throw std::runtime_error{ ERROR_MESSAGE("Program must be run as administrator") };   
        }
        std::cout << "Program is running as administrator" << std::endl;

        CombaseLibrary combase_library{};

        // Create a Scheduled Task to trigger at startup.
        // In the Create Task dialog, select the following:
        // 1. General (tab), Run with highest privileges
        // 2. Triggers (tab), At startup
        // Make the action run the current executable of this program.
        // Use Win32API to do this programmatically.

        // Get the path to the current executable.
        const std::wstring path_to_current_executable = PathToCurrentExecutable{}.GetPath();
        std::wcout << "Path to current executable: " << path_to_current_executable << std::endl;
        AdminStartupItem{}.Create(path_to_current_executable);
        std::cout << "Created startup item successfully" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
