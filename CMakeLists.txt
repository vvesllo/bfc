cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME bfc)
project(${PROJECT_NAME} C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(
	${PROJECT_NAME}
	src/main.c
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})