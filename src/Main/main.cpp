#include <error_message_macro.hpp>
#include <AdminCheck.hpp>
#include <iostream>
#include <ios>
#include <iomanip>

#include <stdexcept>
#include <sstream>
#include <type_traits>
#include <Windows.h>

int main()
{
    try
    {
        AdminCheck admin_check{};
        if (!admin_check.IsAdmin())
        {
            throw std::runtime_error{ ERROR_MESSAGE("Program must be run as administrator") };   
        }
        std::cout << "Program is running as administrator" << std::endl;
        // Use Win32API to register the current process as a service to be run at boot time as administrator
        
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
