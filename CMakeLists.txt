cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(tgui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    add_compile_options("/Zc:__cplusplus")
    add_compile_options("/Zc:preprocessor")
endif(WIN32)

add_subdirectory(lib)
add_subdirectory(demos)