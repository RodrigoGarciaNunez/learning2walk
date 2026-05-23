#include "mujoco_engine.h"

#include "mujoco_view.h"
#include "controller_interface.h"

#include <GLFW/glfw3.h>
#include <mujoco/mujoco.h>


#include <iostream>

mujoco_engine::mujoco_engine(){

    char error[1000];

    m = make_Model("../src/models/humanoid.xml");

    if (!m.get()) {
        std::cout << error << std::endl;
        return;
    }
    d = make_Data();

    
    mujoco_controller_interface = make_unique<controller_interface>(m.get(), d.get());

    //mujoco_controller_interface->py_thread = mujoco_controller_interface->create_thread_SP();

    mujoco_view_= make_unique<mujoco_view>(m.get(), d.get(),  [&](){
                                                                mujoco_controller_interface->simulation_step();
                                                                    });

                                                                    
}

mujoco_engine::~mujoco_engine(){}


int mujoco_engine::start_engine(){

    mujoco_view_->start_window_context();
    mujoco_view_->start_rendering_cicle();
    
    return 0;
}



int mujoco_engine::clean_engine_up(){ 
    // mj_deleteData(d.get());  como tienen su smart pointer, me salo esto
    // mj_deleteModel(m.get());

    mujoco_view_->clean_engine_view();

    return 0;
}



// deleters //////77777777//////////////////////////777


mjModel_SP mujoco_engine::make_Model(const char* path){
    
    mjModel * raw =
        mj_loadXML(path, nullptr, nullptr, 0);

    return mjModel_SP(
        raw,
        mjModel_Deleter{});
}



mjData_SP mujoco_engine::make_Data(){
    
    mjData * raw = mj_makeData(m.get());

    return mjData_SP(
        raw, 
        mjData_Deleter{});
}




void mjModel_Deleter::operator()(mjModel_ * m){
    if(m)
        mj_deleteModel(m);
}


void mjData_Deleter::operator()(mjData_ * d){
    if(d)
        mj_deleteData(d);
}

