#pragma once


#include <memory>

using std::unique_ptr;
using std::make_unique;

using std::make_shared;
using std::shared_ptr;


struct mjModel_;
struct mjData_;

struct mjModel_Deleter{
    void operator()(mjModel_  * m);
};

struct mjData_Deleter{
    void operator()(mjData_  * m);
};


using mjModel_SP = unique_ptr<mjModel_, mjModel_Deleter>;  //mjModel SMART POINTER
using mjData_SP = unique_ptr<mjData_, mjData_Deleter>;  //mjData SMART POINTER

class mujoco_view;
class controller_interface;


class mujoco_engine{
public:
    mujoco_engine();
    virtual ~mujoco_engine();

    int start_engine();
    int clean_engine_up();


private:


    mjModel_SP make_Model(const char* path);
    mjData_SP make_Data();

    unique_ptr<mujoco_view> mujoco_view_;
    unique_ptr<controller_interface> mujoco_controller_interface;



    mjModel_SP m;
    mjData_SP d;


};