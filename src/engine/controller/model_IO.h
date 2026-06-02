#pragma once

#include <array>

using std::array;

#define NUM_JOINTS 21

struct mjData_;

struct model_input{

public:
    model_input(mjData_ *data);
    int model_input_update();


    
    array<float, NUM_JOINTS> joint_pos;
    array<float, NUM_JOINTS> joint_vel;

    array<float, 4> torso_quat;

    array<float, 3> torso_linear_vel;
    array<float, 3> torso_angular_vel;

    bool left_foot_contact;
    bool right_foot_contact;

private:

    mjData_ *d;
};

struct model_output{

    model_output();

    array<float, 21> torque;
};


