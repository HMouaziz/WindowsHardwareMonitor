cmake_minimum_required(VERSION 3.25)
project(WindowsHardwareMonitor)

set(CMAKE_CXX_STANDARD 17)

include_directories(includes)

add_executable(WindowsHardwareMonitor
        includes/HardwareMonitor.hpp
        includes/Logger.hpp
        includes/WMIHandler.hpp
        src/HardwareMonitor.cpp
        src/Logger.cpp
        src/main.cpp
        src/WMIHandler.cpp)

target_link_libraries(WindowsHardwareMonitor Ole32.lib wbemuuid.lib)
