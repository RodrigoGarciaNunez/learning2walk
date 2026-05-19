#pragma once

#include <memory>


using std::unique_ptr;
using std::make_unique;

struct GLFWwindow;

struct GLFWwindow_Deleter{
    void operator()(GLFWwindow * w);
};

using GLFWwindow_SP= unique_ptr<GLFWwindow, GLFWwindow_Deleter>;

class mujoco_engine;
struct mujoco_view_data_;

class mujoco_view{
public:

    mujoco_view();
    virtual ~mujoco_view();
    int start_rendering_cicle();
    int clean_engine_view();




private:

    GLFWwindow_SP make_window();

    unique_ptr<mujoco_view_data_> mujoco_view_data;

    unique_ptr<mujoco_engine> engine;


};