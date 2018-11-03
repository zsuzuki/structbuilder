cmake_minimum_required(VERSION 2.8)
project(struct CXX)
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall")

add_custom_command (
  OUTPUT
    ${PROJECT_SOURCE_DIR}/cpp/struct.hpp
    ${PROJECT_SOURCE_DIR}/cpp/struct_json.cpp
    ${PROJECT_SOURCE_DIR}/cpp/struct_lua.cpp
  COMMAND ${PROJECT_SOURCE_DIR}/structbuilder
    -hpp ${PROJECT_SOURCE_DIR}/cpp/struct.hpp
    -json ${PROJECT_SOURCE_DIR}/cpp/struct_json.cpp
    -lua ${PROJECT_SOURCE_DIR}/cpp/struct_lua.cpp
    ${PROJECT_SOURCE_DIR}/examples/struct.toml
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/examples/struct.toml
)

add_executable(struct cpp/struct.cpp cpp/struct_json.cpp cpp/struct_lua.cpp)
target_link_libraries(struct lua dl)
 
add_custom_target(generate_struct DEPENDS ${PROJECT_SOURCE_DIR}/cpp/struct.hpp)
add_dependencies(struct generate_struct)
