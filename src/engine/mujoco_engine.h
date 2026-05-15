#pragma once


#include <memory>

using std::unique_ptr;
using std::make_unique;



struct mjModel_;
using mjModel = mjModel_;

struct mjData_;
using mjData = mjData_;

struct GLFWwindow;
using GLFWwindow_ = GLFWwindow;

struct mjModel_Deleter{
    void operator()(mjModel  * m);
};

struct mjData_Deleter{
    void operator()(mjData  * m);
};

struct GLFWwindow_Deleter{
    void operator()(GLFWwindow_ * w);
};


using mjModel_SP = unique_ptr<mjModel, mjModel_Deleter>;  //mjModel SMART POINTER
using mjData_SP = unique_ptr<mjData, mjData_Deleter>;  //mjData SMART POINTER
using GLFWwindow_SP = unique_ptr<GLFWwindow_, GLFWwindow_Deleter>;  // GLFWwindow_ SMART POINTER


struct mujoco_data_;


class mujoco_engine{
public:
    mujoco_engine();
    virtual ~mujoco_engine();

    int start_engine();
    void set_mujoco_visualization();
    int start_rendering_cicle();
    int clean_engine_up();


private:


    mjModel_SP make_Model(const char* path);
    mjData_SP make_Data();
    GLFWwindow_SP make_window();



    mjModel_SP m;
    mjData_SP d;
    GLFWwindow_SP window;
    unique_ptr<mujoco_data_> mujoco_data;

};