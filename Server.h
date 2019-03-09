/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.h
 * Author: shahrooz
 *
 * Created on February 28, 2019, 3:22 PM
 */

#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <iostream>
#include "Task.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <map>


using namespace std;

class Server {
public:
    
    Server(string &name, int id, int running_task);
    Server(const Server& orig) = delete;
    virtual ~Server();
    
    int set_running_task(int a);
    void add_done_tasks(int a, bool overwrite = true);
    
    
    int get_status();
    
    string get_name();
    vector <int> get_done_tasks();
    int get_running_task();
    
    string get_server_line();
    
    int get_id();
    
private:
    string name;
    int status; // 0: idle, 1: running
    int id;
    
    vector <int> done_tasks;
    int running_task; // -1 for being idle
    
    void set_status(int a);

};

Task* get_task_by_id(int id, map <int, Task*> *tasks = NULL);

#endif /* SERVER_H */

