#include <string>

// Define a error message macro that combines error message and function name and and file name and line number
#define ERROR_MESSAGE(msg) (std::string(__FUNCTION__) + " in " + std::string(__FILE__) + " at line " + std::to_string(__LINE__) + ": " + msg)

