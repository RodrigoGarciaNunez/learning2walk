#include "controller_interface.h"
#include "mujoco_engine.h"

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>

#include <mujoco/mujoco.h>

using std::cerr;
using std::make_unique;

// pybind utils

using pybind11::class_;
using pybind11::eval_file;
using pybind11::init;
using pybind11::mod_gil_not_used;
using pybind11::module_;
using pybind11::object;
using pybind11::scoped_interpreter;

struct model_input
{
    model_input(mjData_ *d)
    {

        // quaternion
        for (int i = 0; i < 4; i++)
            torso_quat[i] = d->qpos[3 + i];

        // base linear vel
        for (int i = 0; i < 3; i++)
            torso_linear_vel[i] = d->qvel[i];

        // base angular vel
        for (int i = 0; i < 3; i++)
            torso_angular_vel[i] = d->qvel[3 + i];

        // joints
        for (int i = 0; i < NUM_JOINTS; i++)
        {
            joint_pos[i] = d->qpos[7 + i];
            joint_vel[i] = d->qvel[6 + i];
        }
    }

    array<float, NUM_JOINTS> joint_pos;
    array<float, NUM_JOINTS> joint_vel;

    array<float, 4> torso_quat;

    array<float, 3> torso_linear_vel;
    array<float, 3> torso_angular_vel;

    bool left_foot_contact;
    bool right_foot_contact;
};

struct model_output
{
    model_output()
    {
        memset(&torque, 0, sizeof(torque));
    }

    array<float, 21> torque;
};

PYBIND11_EMBEDDED_MODULE(model_IO, m)
{
    class_<model_input>(m, "mujoco_model_IO")
        .def_readwrite("joint_pos", &model_input::joint_pos)
        .def_readwrite("joint_vel", &model_input::joint_vel)
        .def_readwrite("joint_vel", &model_input::joint_vel)
        .def_readwrite("torso_quat", &model_input::torso_quat)
        .def_readwrite("torso_linear_vel", &model_input::torso_linear_vel)
        .def_readwrite("torso_angular_vel", &model_input::torso_angular_vel)
        .def_readwrite("left_foot_contact", &model_input::left_foot_contact)
        .def_readwrite("right_foot_contact", &model_input::right_foot_contact);

    class_<model_output>(m, "ModelOutput")
        .def(init<>())
        .def_readwrite("torque", &model_output::torque);
}

controller_interface::controller_interface(mjModel_ *m, mjData_ *d) : model(m), data(d)
{

    // mujoco_MD = make_unique<mj_model_IO>(m, d);

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

    // num_actuators = actuators_map.size();
    num_actuators = 21;

    map_actuators();

    m_input = make_unique<model_input>(d);
    m_output = make_unique<model_output>();

    py_thread = create_thread_SP();

    cerr << "construido \n";
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

int controller_interface::init_model_IO()
{

    return 0;
}

void controller_interface::simulation_step()
{

    mjtNum simstart = data->time;

    while (data->time - simstart < 1.0 / 60.0)
    {

        for (auto &pair : actuators_map)
        {
            data->ctrl[pair.second.id] = pair.second.torque;
        }

        mj_step(model, data);
    }
}

int controller_interface::get_actuators_torque()
{

    scoped_interpreter guard{};

    pybind11::module_::import("model_IO");

    object scope = module_::import("__main__").attr("__dict__");

    scope["input_mj"] = pybind11::cast(this->m_input.get(), pybind11::return_value_policy::reference);
    scope["output_mj"] = pybind11::cast(this->m_output.get(), pybind11::return_value_policy::reference);


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
