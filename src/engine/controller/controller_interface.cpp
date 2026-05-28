#include "controller_interface.h"
#include "mujoco_engine.h"

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>

#include <iostream>

#include <mujoco/mujoco.h>

using std::cerr;
using std::make_unique;


//pybind utils

using pybind11::mod_gil_not_used;
using pybind11::class_;
using pybind11::scoped_interpreter;
using pybind11::object;
using pybind11::eval_file;
using pybind11::module_;



struct mj_MD{
    mj_MD(mjModel_ * m, mjData_ * d):  model(m), data(d){}
    struct mjModel_ * model;
    struct mjData_  * data;
};

// // preliminar
//  PYBIND11_MODULE(mj_DATA, m, mod_gil_not_used()){
//     class_<mj_MD>(m, "mujoco_info")
//         .def_readwrite("model", &mj_MD::model)
//         .def_readwrite("data", &mj_MD::data);
//     }
    



struct example{
    example(int i=0, int j=1): i(i), j(j) {}
    int i;
    int j;
};


 PYBIND11_EMBEDDED_MODULE(module_example, m){
    class_<example>(m, "example_")
        .def(pybind11::init<>())
        .def_readwrite("i", &example::i)
        .def_readwrite("j", &example::j);
    }
    


controller_interface::controller_interface(mjModel_ *m, mjData_ *d) //: model(m), data(d)
{

    mujoco_MD = make_unique<mj_MD>(m, d);
    ex = make_unique<example>();

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

    py_thread = create_thread_SP();

    cerr << "construido \n";
}

int controller_interface::map_actuators()
{

    const char *aux;

    for (auto &pair : actuators_map)
    {
        aux = pair.first.c_str();
        pair.second.id = mj_name2id(mujoco_MD->model, mjOBJ_ACTUATOR, aux);
    }

    return 0;
}

controller_interface::~controller_interface()
{
}

void controller_interface::simulation_step()
{

    mjtNum simstart = mujoco_MD->data->time;

    while (mujoco_MD->data->time - simstart < 1.0 / 60.0)
    {

        // get_actuators_torque();

        for (auto &pair : actuators_map)
        {
            mujoco_MD->data->ctrl[pair.second.id] = pair.second.torque;
        }

        mj_step(mujoco_MD->model,mujoco_MD->data);
    }
}

int controller_interface::get_actuators_torque()
{
   
    scoped_interpreter guard{};

    pybind11::module_::import("module_example");
    
    object scope = module_::import("__main__").attr("__dict__");

    scope["example"] = pybind11::cast(this->ex.get(),  pybind11::return_value_policy::reference);


    try
    {
        pybind11::eval_file(script_path, scope);
    }
    catch (const pybind11::error_already_set &e)
    {
        PyErr_Print();
        return 1;
    }
    return 0;
}

// int controller_interface::get_actuators_torque()
// {

//     py_imp = make_unique<Pyimp>();

//     unique_ptr<FILE> file(fopen(script_path, "r"));

//     if (file == nullptr)
//     {

//         cerr << "No se pudo abrir el script\n";
//         return -1;
//     }

//     PyConfig_InitPythonConfig(&(py_imp->config));
//     py_imp->status = PyConfig_SetBytesString(&(py_imp->config), &(py_imp->config).program_name, script_path);
//     if (PyStatus_Exception(py_imp->status))
//     {
//         goto exception;
//     }
//     py_imp->status = Py_InitializeFromConfig(&(py_imp->config));
//     if (PyStatus_Exception(py_imp->status))
//     {
//         goto exception;
//     }
//     PyConfig_Clear(&(py_imp->config));

//     PyRun_SimpleFile(
//         file.get(),
//         script_path);

//     if (Py_FinalizeEx() < 0)
//     {
//         exit(120);
//     }
//     return 0;

// exception:
//     PyConfig_Clear(&(py_imp->config));
//     Py_ExitStatusException(py_imp->status);

//     return 0;
// }

thread_SP controller_interface::create_thread_SP()
{
    thread *t = new thread(&controller_interface::get_actuators_torque, this);
    cerr << "voy bien \n";

    return thread_SP(t, thread_deleter{});
}

void thread_deleter::operator()(thread *t)
{
    if (t && t->joinable())
    {
        t->join();
    }
}
