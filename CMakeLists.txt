
cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

project(learning2Walk)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

add_executable(main src/main.cpp) 

add_library(engine src/engine/mujoco_engine.cpp)

target_include_directories(engine PRIVATE   #es private para que quien incluya a engine no "herede" el directorio
    $ENV{MUJOCO_INCLUDE}
)

find_library(MUJOCO_LIB
    NAMES mujoco
    PATHS $ENV{MUJOCO_LIB}
)

find_package(glfw3 REQUIRED)

target_link_libraries(engine PRIVATE
    glfw
    ${MUJOCO_LIB}
)


target_link_libraries(main PRIVATE
    engine
)
