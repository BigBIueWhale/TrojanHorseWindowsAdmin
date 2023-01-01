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
    AdminCheck admin_check{};
    std::cout << "Is running as admin: " << std::flush << std::boolalpha << IsRunningAsAdmin() << std::noboolalpha << std::endl;
    return 0;
}
