cmake_minimum_required(VERSION 3.10)

# Project Name
project(SimpleChatServerClient)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable for the server
add_executable(server ../server.cpp)

# Add the executable for the client
add_executable(client ../client.cpp)

# Link pthread for threading support
target_link_libraries(server pthread)
