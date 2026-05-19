#pragma once


#include <memory>

using std::unique_ptr;
using std::make_unique;



struct mjModel_;
struct mjData_;
struct GLFWwindow;

struct mjModel_Deleter{
    void operator()(mjModel_  * m);
};

struct mjData_Deleter{
    void operator()(mjData_  * m);
};

// struct GLFWwindow_Deleter{
//     void operator()(GLFWwindow * w);
// };


using mjModel_SP = unique_ptr<mjModel_, mjModel_Deleter>;  //mjModel SMART POINTER
using mjData_SP = unique_ptr<mjData_, mjData_Deleter>;  //mjData SMART POINTER
//using GLFWwindow_SP = unique_ptr<GLFWwindow, GLFWwindow_Deleter>;  // GLFWwindow_ SMART POINTER


struct mujoco_data_;


class mujoco_engine{
public:
    mujoco_engine();
    virtual ~mujoco_engine();

    int start_engine();
    //void set_mujoco_visualization();
    int start_rendering_cicle();
    int clean_engine_up();


private:


    mjModel_SP make_Model(const char* path);
    mjData_SP make_Data();
    //GLFWwindow_SP make_window();



    mjModel_SP m;
    mjData_SP d;
    //GLFWwindow_SP window;
    //unique_ptr<mujoco_data_> mujoco_data;

};