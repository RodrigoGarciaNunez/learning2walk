
cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

project(learning2Walk)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

add_executable(main src/main.cpp) 

add_library(engine
    src/engine/mujoco_engine.cpp
    src/engine/view/mujoco_view.cpp
    src/engine/controller/controller_interface.cpp
)

target_include_directories(engine PRIVATE   #es private para que quien incluya a engine no "herede" el directorio
    $ENV{MUJOCO_INCLUDE}
    src/engine
    src/engine/view
    src/engine/controller
    #/usr/include/python3.14
)


find_library(MUJOCO_LIB
    NAMES mujoco
    PATHS $ENV{MUJOCO_LIB}
)

find_package(glfw3 REQUIRED)

set(Python3_EXECUTABLE ${PYTHON_VENV_EXEC})

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

message(STATUS ${Python3_EXECUTABLE})
message(STATUS ${Python3_INCLUDE_DIRS})
message(STATUS ${Python3_LIBRARIES})


target_link_libraries(engine PRIVATE
    glfw
    ${MUJOCO_LIB}
    Python3::Python
    #engine_view
    #engine_controller
)

#target_link_libraries(engine_view PRIVATE engine)
#target_link_libraries(engine_controller PRIVATE engine)
target_link_libraries(main PRIVATE engine)


