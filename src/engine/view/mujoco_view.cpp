#include "mujoco_view.h"

#include <GLFW/glfw3.h>
#include <mujoco/mujoco.h>


struct mujoco_view_data_{
    mjvCamera cam;
    mjvOption opt;
    mjvScene scn;
    mjrContext con;
};


mujoco_view::mujoco_view(){

    mujoco_view_data = make_unique<mujoco_view_data_>(); 

    
    mjv_defaultCamera(&(mujoco_view_data->cam));
    mjv_defaultOption(&(mujoco_view_data->opt));

    mjv_defaultScene(&(mujoco_view_data->scn));
    mjr_defaultContext(&(mujoco_view_data->con));

    mjv_makeScene(m.get(), &(mujoco_view_data->scn), 2000);
    mjr_makeContext(m.get(), &(mujoco_view_data->con), mjFONTSCALE_150);
}

virtual mujoco_view::~mujoco_view(){}


int mujoco_view::start_rendering_cicle(){
    
    while (!glfwWindowShouldClose(window.get())) {

        mj_step(m.get(), d.get());

        mjrRect viewport =
        {
            0,
            0,
            1200,
            900
        };

        mjv_updateScene(
            m.get(),
            d.get(),
            &(mujoco_view_data->opt),
            NULL,
            &(mujoco_view_data->cam),
            mjCAT_ALL,
            &(mujoco_view_data->scn)
        );

        mjr_render(viewport, &(mujoco_view_data->scn), &(mujoco_view_data->con));

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    return 0;
}


int mujoco_view::clean_engine_view(){
    mjv_freeScene(&(mujoco_view_data->scn));
    mjr_freeContext(&(mujoco_view_data->con));

    glfwTerminate();
}




GLFWwindow_SP mujoco_view::make_window(){
    
    GLFWwindow* raw =
        glfwCreateWindow(
            1200,
            900,
            "MuJoCo",
            NULL,
            NULL
        );

    return GLFWwindow_SP(raw, GLFWwindow_Deleter{});
}


void GLFWwindow_Deleter::operator()(GLFWwindow * w){
    if(w)
        glfwDestroyWindow(w);
}