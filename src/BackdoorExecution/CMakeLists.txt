add_library(BackdoorExecution STATIC BackdoorExecution.cpp BackdoorExecution.hpp)
target_link_libraries(BackdoorExecution PRIVATE UdpServer ProtoMessages cryptb)
