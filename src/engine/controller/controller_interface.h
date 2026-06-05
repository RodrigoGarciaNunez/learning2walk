#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <utility>
#include <memory>
#include <thread>
#include <vector>
#include <cstring>
#include <mutex>

#define PY_SSIZE_T_CLEAN
#define NUM_JOINTS 21

using std::string;
using std::unordered_map;
using std::unique_ptr;
using std::thread;
using std::array;
using std::vector;
using std::memset;
using std::mutex;
using std::unique_lock;
using std::lock_guard;

struct mjModel_;
struct mjData_;


struct actuator_{
    int id=-1;
    int torque=1.0;
};

struct thread_deleter{
    void operator()(thread * t);
};

using thread_SP = unique_ptr<thread , thread_deleter>;



struct  model_input;
using model_input_ = unique_ptr<model_input>;

struct  model_output;
using model_output_ =unique_ptr<model_output>;

class controller_interface{
public:

    controller_interface(mjModel_ * m, mjData_ * d);
    virtual ~controller_interface();
    void simulation_step();
    
    

private:

    int map_actuators();
    int get_actuators_torque();  //levanta el scope de python
       
    thread_SP create_thread_SP();
    
    
    mjModel_ * model;
    mjData_ * data;
    
    unordered_map<const char *, actuator_> actuators_map;
    int num_actuators;


    ///// Python ///////

   //este hilo se encarga de soportar el python listener

    thread_SP py_thread;  
    const char * script_path= "../src/scripts/actions_server.py";


   model_input_ m_input;
   model_output_ m_output;

    mutex mutex_;
    unique_ptr<unique_lock<mutex>> model_data_locker;
    


    
};