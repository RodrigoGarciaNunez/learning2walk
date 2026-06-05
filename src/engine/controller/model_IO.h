#pragma once

#include <array>
#include <unordered_map>
#include <string>


using std::array;
using std::unordered_map;
using std::string;

#define NUM_JOINTS 21

struct mjData_;
struct mjModel_;

struct model_input{

public:
    model_input(mjData_ *data, mjModel_ *model);
    int model_input_update();


    
    array<float, NUM_JOINTS> joint_pos;
    array<float, NUM_JOINTS> joint_vel;

    array<float, 4> torso_quat;

    array<float, 3> torso_linear_vel;
    array<float, 3> torso_angular_vel;

    int left_foot_contact;
    int right_foot_contact;

private:

    unordered_map<int, const char *> foots_dict;

    mjData_ *d;
    mjModel_ *m;
};

struct model_output{

    model_output();

    array<float, 21> torque;
};


