cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project("dork")
add_executable("dork" "main.cpp" "tokens.cpp" "objects.cpp" "room.cpp" "game_state.cpp" "files.cpp")

target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})
install(TARGETS ${PROJECT_NAME} DESTINATION ".")

install(DIRECTORY ../worlds DESTINATION ".")
install(DIRECTORY DESTINATION saves)
install(DIRECTORY DESTINATION saves/door_world)

add_subdirectory(objects)
