#include "controller_interface.h"

#include "mujoco_engine.h"

#include <mujoco/mujoco.h>




controller_interface::controller_interface(mjModel_ * m, mjData_ * d):model(m), data(d){

    actuators_map = {{"abdomen_z",    {}},                     
                    {"abdomen_y",           {}},       
                    {"abdomen_x",           {}}, 
                    {"hip_x_right",        {}}, 
                    {"hip_z_right",         {}}, 
                    {"hip_y_right" ,        {}}, 
                    {"knee_right",          {}}, 
                    {"ankle_y_right",       {}}, 
                    {"ankle_x_right",       {}}, 
                    {"hip_x_left",          {}}, 
                    {"hip_z_left",          {}}, 
                    {"hip_y_left",          {}}, 
                    {"knee_left",           {}}, 
                    {"ankle_y_left",        {}}, 
                    {"ankle_x_left",        {}}, 
                    {"shoulder1_right",     {}}, 
                    {"shoulder2_right",     {}}, 
                    {"elbow_right",         {}}, 
                    {"shoulder1_left",      {}}, 
                    {"shoulder2_left",      {}}, 
                    {"elbow_left",         {}}};



    map_actuators();

    
}

int controller_interface::map_actuators(){
    
    const char * aux;
    
    for (auto & pair : actuators_map){
        aux = pair.first.c_str();
        pair.second.id =  mj_name2id(model,mjOBJ_ACTUATOR,aux);
    }
      
    return 0;
}



controller_interface::~controller_interface(){

}


int controller_interface::simulation_step(){

    while(true){

        for(auto &pair : actuators_map){
            data->ctrl[pair.second.id] = 0;
        }

        mj_step(model, data);
    }
}

