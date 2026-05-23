#pragma once

#include <memory>
#include <functional>


using std::unique_ptr;
using std::make_unique;
using std::function;

struct GLFWwindow;

struct GLFWwindow_Deleter{
    void operator()(GLFWwindow * w);
};

using GLFWwindow_SP= unique_ptr<GLFWwindow, GLFWwindow_Deleter>;
struct mujoco_view_data_;

struct mjModel_;
struct mjData_;

class controller_interface;

class mujoco_view{
public:

    mujoco_view(mjModel_*  m, mjData_*  d, function<void()> step_call);   
    virtual ~mujoco_view();
    int start_window_context();
    int start_rendering_cicle();
    int clean_engine_view();
    
    


private:

    GLFWwindow_SP make_window();

    unique_ptr<mujoco_view_data_> mujoco_view_data;
    
    mjModel_* model;
    mjData_* data;
    GLFWwindow_SP window;


    function<void()> step_callback;


};