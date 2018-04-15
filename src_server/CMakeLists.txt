file(GLOB_RECURSE SRC_FILES *.cpp)
include_directories(${PROJECT_SOURCE_DIR}/src_server)
add_executable(server ${SRC_FILES})
