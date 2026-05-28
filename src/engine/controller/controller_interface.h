#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <utility>
#include <memory>
#include <thread>

#define PY_SSIZE_T_CLEAN

using std::string;
using std::unordered_map;
using std::unique_ptr;
using std::thread;

struct mjModel_;
struct mjData_;

struct mj_MD;
struct example;
// struct PyStatus; Son NO opacos, por lo que no se les puede hacer forward declaration tradicional
// struct PyConfig;

struct actuator_{
    int id=-1;
    int torque=1.0;
};

struct thread_deleter{
    void operator()(thread * t);
};

using thread_SP = unique_ptr<thread , thread_deleter>;


class controller_interface{
public:

    controller_interface(mjModel_ * m, mjData_ * d);
    virtual ~controller_interface();
    void simulation_step();
    
    

private:

    int map_actuators();
    int get_actuators_torque();  //en esta se debe tener la api para el modelo de python
       
    thread_SP create_thread_SP();
    
    
    // mjModel_ * model;
    // mjData_ * data;
    
    unique_ptr<mj_MD> mujoco_MD;
    unique_ptr<example> ex;

    unordered_map<string, actuator_> actuators_map;


    ///// Python ///////

   //este hilo se encarga de soportar el python listener

    thread_SP py_thread;  
    const char * script_path= "../src/scripts/actions_server.py";
    
};