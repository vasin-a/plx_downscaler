cmake_minimum_required(VERSION 3.20)

project(host LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC
	args-parser.cpp
	args-parser.hpp
	main.cpp
	pch.hpp
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC})
add_executable(${PROJECT_NAME} ${SRC})

target_precompile_headers(${PROJECT_NAME} PUBLIC pch.hpp)
target_link_libraries(${PROJECT_NAME} PRIVATE lib)
target_link_libraries(${PROJECT_NAME} PRIVATE shared)
