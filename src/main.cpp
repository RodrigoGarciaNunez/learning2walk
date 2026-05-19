#include <iostream>
#include <memory>

#include "engine/mujoco_engine.h"

using std::unique_ptr;
using std::make_unique;
    
using std::shared_ptr;
using std::make_shared;

int main() {
    unique_ptr<mujoco_engine> me = make_unique<mujoco_engine>();
    me->start_engine();
    //me->set_mujoco_visualization();
    //me->start_rendering_cicle();
    me->clean_engine_up();
    return 0;
} 


//mjModel* m = nullptr;
//mjData* d = nullptr;

// char error[1000];

    // m = mj_loadXML("../src/models/humanoid.xml", 0, error, 1000);

    // if (!m) {
    //     std::cout << error << std::endl;
    //     return 1;
    // }

    // d = mj_makeData(m);

    // //-----------------------------------
    // // GLFW init
    // //-----------------------------------

    // if (!glfwInit()) {
    //     return 1;
    // }

    // GLFWwindow* window =
    //     glfwCreateWindow(
    //         1200,
    //         900,
    //         "MuJoCo",
    //         NULL,
    //         NULL
    //     );

    // glfwMakeContextCurrent(window);

    // //-----------------------------------
    // // MuJoCo visualization
    // //-----------------------------------

    // mjvCamera cam;
    // mjvOption opt;
    // mjvScene scn;
    // mjrContext con;

    // mjv_defaultCamera(&cam);
    // mjv_defaultOption(&opt);

    // mjv_defaultScene(&scn);
    // mjr_defaultContext(&con);

    // mjv_makeScene(m, &scn, 2000);
    // mjr_makeContext(m, &con, mjFONTSCALE_150);

    // //-----------------------------------
    // // Main loop
    // //-----------------------------------

    // while (!glfwWindowShouldClose(window)) {

    //     mj_step(m, d);

    //     mjrRect viewport =
    //     {
    //         0,
    //         0,
    //         1200,
    //         900
    //     };

    //     mjv_updateScene(
    //         m,
    //         d,
    //         &opt,
    //         NULL,
    //         &cam,
    //         mjCAT_ALL,
    //         &scn
    //     );

    //     mjr_render(viewport, &scn, &con);

    //     glfwSwapBuffers(window);
    //     glfwPollEvents();
    // }

    // //-----------------------------------
    // // Cleanup
    // //-----------------------------------

    // mj_deleteData(d);
    // mj_deleteModel(m);

    // mjv_freeScene(&scn);
    // mjr_freeContext(&con);

    // glfwTerminate();
