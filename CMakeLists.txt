cmake_minimum_required(VERSION 3.5)

project(UDPTool)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find Qt5 (adjust if using Qt6 or different version)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

# Add executable
add_executable(UDPTool src/main.cpp src/UDPTool.cpp)

# Link with Qt libraries
target_link_libraries(UDPTool Qt6::Core Qt6::Widgets Qt6::Network)
