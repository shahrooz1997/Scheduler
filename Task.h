/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Task.h
 * Author: shahrooz
 *
 * Created on February 28, 2019, 3:21 PM
 */

#ifndef TASK_H
#define TASK_H

#include <string>
#include <iostream>
//#include "Server.h"
#include <sstream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>


using namespace std;

class Task {
public:
    
    Task(string &cache_type, string &benchmark, string &mem_fraction, int id, int runner_server, int status);
    Task(const Task& orig) = delete;
    virtual ~Task();
    
    string get_name();
    
    string get_task_line();
    
    int set_runner_server(int a);
    int get_status();
    int set_status(int a);
    
    string get_cache_type();
    string get_benchmark();
    string get_mem_fraction();
    
    int get_id();
    
private:
    string name;
    string cache_type;
    string benchmark;
    string mem_fraction;
    int id;
    
    int runner_server;
    
    int status; // 0: waiting, 1: running, 2: done

};

#endif /* TASK_H */

