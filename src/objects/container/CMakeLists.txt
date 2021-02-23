project(container)

include(GenerateExportHeader)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(${PROJECT_NAME} SHARED container.cpp)

install(TARGETS ${PROJECT_NAME} DESTINATION "objects")

generate_export_header(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	# G++
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	# MSVC
	target_compile_options(${PROJECT_NAME} PRIVATE /EHsc /MTd /W2 /c)
	# Set the DLLEXPORT variable to export symbols
	target_compile_definitions(${PROJECT_NAME} PRIVATE WIN_EXPORT)
endif()
