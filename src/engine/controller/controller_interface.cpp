
#include "controller_interface.h"
#include "mujoco_engine.h"
#include "model_IO.h"

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
//#include <pybind11/stl.h>

#include <iostream>

#include <mujoco/mujoco.h>

using std::cerr;
using std::make_unique;

// pybind utils

using pybind11::eval_file;
using pybind11::module_;
using pybind11::object;
using pybind11::scoped_interpreter;
using pybind11::cast;
using pybind11::return_value_policy;
using pybind11::error_already_set;

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

    num_actuators = 21;

    map_actuators();

    m_input = make_unique<model_input>(d);
    m_output = make_unique<model_output>();

    py_thread = create_thread_SP();

}

controller_interface::~controller_interface()
{
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



void controller_interface::simulation_step()
{

    mjtNum simstart = data->time;

    m_input->model_input_update();

    while (data->time - simstart < 1.0 / 60.0)
    {

        for (auto &pair : actuators_map)
        {
            data->ctrl[pair.second.id] = m_output->torque[pair.second.id];
        }

        mj_step(model, data);
    }
}

int controller_interface::get_actuators_torque()
{

    scoped_interpreter guard{};

    module_::import("model_IO");

    object scope = module_::import("__main__").attr("__dict__");

    scope["input_mj"] = cast(this->m_input.get(), return_value_policy::reference);
    scope["output_mj"] = cast(this->m_output.get(), return_value_policy::reference);


    try
    {
        eval_file(script_path, scope);
    }
    catch (const error_already_set &e)
    {
        PyErr_Print();
        return 1;
    }
    return 0;
}

thread_SP controller_interface::create_thread_SP()
{
    thread *t = new thread(&controller_interface::get_actuators_torque, this);

    return thread_SP(t, thread_deleter{});
}

void thread_deleter::operator()(thread *t)
{
    if (t && t->joinable())
    {
        t->detach();
        delete(t);
    }
}
