cmake_minimum_required(VERSION 3.10)
project(encoder_application)
set(CMAKE_CXX_STANDART 11)
set(CMAKE_CXX_STANDART_REQUIRED ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../source source_dir)
add_executable(encoder_application ${CMAKE_CURRENT_SOURCE_DIR}/d32.cpp)
target_include_directories(source PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/../source)
target_link_libraries(encoder_application source)



