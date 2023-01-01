add_library(AdminCheck STATIC AdminCheck.cpp AdminCheck.hpp)
target_link_libraries(AdminCheck PRIVATE Win32ErrorMessage)
