#include <error_message_macro.hpp>
#include <AdminCheck.hpp>
#include <iostream>
#include <ios>
#include <iomanip>

#include <stdexcept>
#include <sstream>
#include <type_traits>

int main()
{
    try
    {
        AdminCheck admin_check{};
        std::cout << "Is running as admin: " << std::flush << std::boolalpha << admin_check.IsAdmin() << std::noboolalpha << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
