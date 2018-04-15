cmake_minimum_required(VERSION 3.0)
project("Socket Programming Notes")

set(EXTERNALS_DIR externals)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++14 -pthread")

# add_subdirectory("${EXTERNALS_DIR}/googletest")

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 COMPONENTS filesystem thread) 

# if(Boost_FOUND)
# 	include_directories(${Boost_INCLUDE_DIRS})
# 	add_subdirectory(src)
# 	enable_testing()
# 	add_subdirectory(test)
# else()
# 	message("Boost not found! Compilation will fail. Please download version 1.45.0 or above")
# endif()

add_subdirectory(src_server)
add_subdirectory(src_client)