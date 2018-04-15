file(GLOB_RECURSE SRC_FILES *.cpp)
include_directories(${PROJECT_SOURCE_DIR}/src_client)
add_executable(client ${SRC_FILES})