#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <utility>


using std::array;
using std::string;
using std::unordered_map;
using std::pair;

struct mjModel_;
struct mjData_;

struct actuator_{
    int id=-1;
    int torque=0.0;
};

class controller_interface{
public:

    controller_interface(mjModel_ * m, mjData_ * d);
    virtual ~controller_interface();

    int simulation_step();


private:

    int map_actuators();
    int get_actuators_torque();  //en esta se debe tener la api para el modelo de python
    

    mjModel_ * model;
    mjData_ * data;    

    unordered_map<string, actuator_> actuators_map;
};