#include "model_IO.h"
#include <mujoco/mujoco.h>

#include <array>
#include <cstring>

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using std::array;
using std::memset;

using pybind11::class_;
using pybind11::init;

model_input::model_input(mjData_ *data):d(data)
{
    model_input_update();
}

int model_input::model_input_update()
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

    return 0;
}


model_output::model_output()
{
    memset(&torque, 0, sizeof(torque));
}

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