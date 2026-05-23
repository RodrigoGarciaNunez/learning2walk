#include "controller_interface.h"
#include "mujoco_engine.h"

#include <iostream>

#include <mujoco/mujoco.h>

#include <Python.h>

using std::make_unique;
using std::cerr;

struct controller_interface::Pyimp
{
    PyStatus status;
    PyConfig config;
};

controller_interface::controller_interface(mjModel_ *m, mjData_ *d) : model(m), data(d)
{

    actuators_map = {{"abdomen_z", {}},
                     {"abdomen_y", {}},
                     {"abdomen_x", {}},
                     {"hip_x_right", {}},
                     {"hip_z_right", {}},
                     {"hip_y_right", {}},
                     {"knee_right", {}},
                     {"ankle_y_right", {}},
                     {"ankle_x_right", {}},
                     {"hip_x_left", {}},
                     {"hip_z_left", {}},
                     {"hip_y_left", {}},
                     {"knee_left", {}},
                     {"ankle_y_left", {}},
                     {"ankle_x_left", {}},
                     {"shoulder1_right", {}},
                     {"shoulder2_right", {}},
                     {"elbow_right", {}},
                     {"shoulder1_left", {}},
                     {"shoulder2_left", {}},
                     {"elbow_left", {}}};

    //

    map_actuators();

    py_thread= create_thread_SP();
   
    cerr << "construido \n";

}

int controller_interface::map_actuators()
{

    const char *aux;

    for (auto &pair : actuators_map)
    {
        aux = pair.first.c_str();
        pair.second.id = mj_name2id(model, mjOBJ_ACTUATOR, aux);
    }

    return 0;
}

controller_interface::~controller_interface()
{
}

void controller_interface::simulation_step()
{

    mjtNum simstart = data->time;

    while (data->time - simstart < 1.0 / 60.0)
    {

        //get_actuators_torque();

        for (auto &pair : actuators_map)
        {
            data->ctrl[pair.second.id] = pair.second.torque;
        }

        mj_step(model, data);
    }
}

int controller_interface::get_actuators_torque()
{

    py_imp = make_unique<Pyimp>();
    
    unique_ptr<FILE> file(fopen(script_path, "r"));

    if (file == nullptr)
    {

        cerr << "No se pudo abrir el script\n";
        return -1;
    }

    PyConfig_InitPythonConfig(&(py_imp->config));
    py_imp->status = PyConfig_SetBytesString(&(py_imp->config), &(py_imp->config).program_name, script_path);
    if (PyStatus_Exception(py_imp->status))
    {
        goto exception;
    }
    py_imp->status = Py_InitializeFromConfig(&(py_imp->config));
    if (PyStatus_Exception(py_imp->status))
    {
        goto exception;
    }
    PyConfig_Clear(&(py_imp->config));

    PyRun_SimpleFile(
        file.get(),
        script_path);


    // PyRun_SimpleString(
    //     "import sys\n"
    //     "from time import time,ctime\n"
    //     "print('Today is', ctime(time()))\n"
    //     "print('version:', sys.version)\n");

    if (Py_FinalizeEx() < 0)
    {
        exit(120);
    }
    return 0;

exception:
    PyConfig_Clear(&(py_imp->config));
    Py_ExitStatusException(py_imp->status);

    return 0;
}



int controller_interface::hola(){
    cerr <<"hola \n";
    return 0;
}

thread_SP controller_interface::create_thread_SP(){
    thread * t = new thread(&controller_interface::get_actuators_torque, this);
    cerr << "voy bien \n";
    
    return thread_SP(t, thread_deleter{});
}


void thread_deleter::operator()(thread * t){
    if(t && t->joinable()){
        t->join();
    }
}
