cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project ("dork")
add_executable("dork" "main.cpp" "tokens.cpp" "objects.cpp" "room.cpp")
