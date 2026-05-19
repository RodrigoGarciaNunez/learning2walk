#include "mujoco_engine.h"

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

    //mujoco_data = make_unique<mujoco_data_>(); -> new mujoco_view()
}

mujoco_engine::~mujoco_engine(){}


int mujoco_engine::start_engine(){

    if (!glfwInit()) {
        return 1;
    }
   //window = make_window(); -> mujoco_view-> make_window()
    

    //glfwMakeContextCurrent(window.get());

    


    return 0;
}

// void mujoco_engine::set_mujoco_visualization(){
    
//     mjv_defaultCamera(&(mujoco_data->cam));
//     mjv_defaultOption(&(mujoco_data->opt));

//     mjv_defaultScene(&(mujoco_data->scn));
//     mjr_defaultContext(&(mujoco_data->con));

//     mjv_makeScene(m.get(), &(mujoco_data->scn), 2000);
//     mjr_makeContext(m.get(), &(mujoco_data->con), mjFONTSCALE_150);
// }


// int mujoco_engine::start_rendering_cicle(){
    
//     while (!glfwWindowShouldClose(window.get())) {

//         mj_step(m.get(), d.get());

//         mjrRect viewport =
//         {
//             0,
//             0,
//             1200,
//             900
//         };

//         mjv_updateScene(
//             m.get(),
//             d.get(),
//             &(mujoco_data->opt),
//             NULL,
//             &(mujoco_data->cam),
//             mjCAT_ALL,
//             &(mujoco_data->scn)
//         );

//         mjr_render(viewport, &(mujoco_data->scn), &(mujoco_data->con));

//         glfwSwapBuffers(window.get());
//         glfwPollEvents();
//     }

//     return 0;
// }

int mujoco_engine::clean_engine_up(){ 
    // mj_deleteData(d.get());  como tienen su smart pointer, me salo esto
    // mj_deleteModel(m.get());

    // mjv_freeScene(&(mujoco_data->scn));  deben ser llamados desde mujoco_view
    // mjr_freeContext(&(mujoco_data->con));

    glfwTerminate();

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


// GLFWwindow_SP mujoco_engine::make_window(){
    
//     GLFWwindow* raw =
//         glfwCreateWindow(
//             1200,
//             900,
//             "MuJoCo",
//             NULL,
//             NULL
//         );

//     return GLFWwindow_SP(raw, GLFWwindow_Deleter{});
//}


void mjModel_Deleter::operator()(mjModel_ * m){
    if(m)
        mj_deleteModel(m);
}


void mjData_Deleter::operator()(mjData_ * d){
    if(d)
        mj_deleteData(d);
}


// void GLFWwindow_Deleter::operator()(GLFWwindow * w){
//     if(w)
//         glfwDestroyWindow(w);
// }