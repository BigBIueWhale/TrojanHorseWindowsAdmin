add_library(ExecutableRun STATIC ExecutableRun.cpp ExecutableRun.hpp)
target_link_libraries(ExecutableRun PRIVATE Win32ErrorMessage)
