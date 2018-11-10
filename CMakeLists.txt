cmake_minimum_required(VERSION 2.8)
# struct builder test program
project(struct CXX)
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall")
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_custom_command (
  OUTPUT
    ${PROJECT_SOURCE_DIR}/cpp/struct.hpp
    ${PROJECT_SOURCE_DIR}/cpp/struct_json.cpp
    ${PROJECT_SOURCE_DIR}/cpp/struct_ser.cpp
    ${PROJECT_SOURCE_DIR}/cpp/struct_lua.cpp
  COMMAND go run ${PROJECT_SOURCE_DIR}/main.go
    -hpp ${PROJECT_SOURCE_DIR}/cpp/struct.hpp
    -json ${PROJECT_SOURCE_DIR}/cpp/struct_json.cpp
    -serialize ${PROJECT_SOURCE_DIR}/cpp/struct_ser.cpp
    -lua ${PROJECT_SOURCE_DIR}/cpp/struct_lua.cpp
    -template ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/examples/struct.toml
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/examples/struct.toml
  DEPENDS
    ${PROJECT_SOURCE_DIR}/templates/struct_json.tpl
    ${PROJECT_SOURCE_DIR}/templates/struct_json_child_in.tpl
    ${PROJECT_SOURCE_DIR}/templates/struct_json_child_out.tpl
    ${PROJECT_SOURCE_DIR}/templates/struct_ser.tpl
    ${PROJECT_SOURCE_DIR}/templates/struct_ser_child_in.tpl
    ${PROJECT_SOURCE_DIR}/templates/struct_ser_child_out.tpl
    ${PROJECT_SOURCE_DIR}/templates/struct_ser_child_size.tpl
    ${PROJECT_SOURCE_DIR}/templates/luasol.tpl
    ${PROJECT_SOURCE_DIR}/templates/luasol_child.tpl
    ${PROJECT_SOURCE_DIR}/templates/struct_hpp.tpl
    ${PROJECT_SOURCE_DIR}/makestruct/struct.go
    ${PROJECT_SOURCE_DIR}/serialize/serialize.go
    ${PROJECT_SOURCE_DIR}/main.go
)

add_executable(struct cpp/struct.cpp cpp/struct_json.cpp cpp/struct_lua.cpp cpp/struct_ser.cpp)
target_link_libraries(struct lua dl)
 
add_custom_target(generate_struct DEPENDS ${PROJECT_SOURCE_DIR}/cpp/struct.hpp)
add_dependencies(struct generate_struct)

# serializer test program
project(sample CXX)
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall")
add_executable(sample cpp/sample.cpp)
