cmake_minimum_required(VERSION 3.14)
project(cpp-project CXX)

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(cpp-project ${SRC_FILES})
target_include_directories(cpp-project PUBLIC "${CMAKE_SOURCE_DIR}/include")
set_target_properties(cpp-project PROPERTIES CXX_STANDARD 23)

set(SFML_STATIC_LIBRARIES TRUE)
set(CMAKE_PREFIX_PATH "D:/SFML/SFML-2.5.1/lib/cmake/")
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
target_include_directories(cpp-project PUBLIC "${SFML_DIR}/include")
target_link_directories(cpp-project PUBLIC "${SFML_DIR}/lib")
target_link_libraries(cpp-project sfml-graphics)