cmake_minimum_required(VERSION 3.12)

project(SnakeAI VERSION 0.0.1)

find_package(PkgConfig)
pkg_check_modules(GLFW3 REQUIRED IMPORTED_TARGET glfw3)
pkg_check_modules(GLEW REQUIRED IMPORTED_TARGET glew)

set(target_name                 SnakeAI)
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} -Wno-error=format-security")
set(USE_MSVC_RUNTIME_LIBRARY    OFF)

file(GLOB src
    src/*.cpp
    src/**/*.cpp
    src/**/**/*.cpp
    src/**/**/**/*.cpp
    src/**/**/**/**/*.cpp
)

add_executable(${target_name} ${src})
target_include_directories(${target_name} PUBLIC src src/external)
target_precompile_headers(${target_name} PUBLIC src/pch.h)
target_compile_options(${target_name} PUBLIC)
target_link_libraries(${target_name} 
    PkgConfig::GLFW3
    PkgConfig::GLEW
)

install(TARGETS ${target_name} DESTINATION bin)


