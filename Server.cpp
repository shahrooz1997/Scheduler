/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.cpp
 * Author: shahrooz
 * 
 * Created on February 28, 2019, 3:22 PM
 */

#include "Server.h"

Server::Server(string &name, int id, int running_task) {
    this->name = name;
    this->id = id;
    this->running_task = running_task;
    if(running_task == -1){
        this->status = 0; // idle
    }
    else{
        this->status = 1; // running
    }
}


Server::~Server() {
    done_tasks.clear();
}

int Server::set_running_task(int a){
    if(a != -1){
        if(running_task != -1){
            cerr << "The server is running the task number " << running_task
                    << "and it is not possible to assign the new task " << a << endl;
            return -1;
        }
        this->running_task = a;
        this->set_status(1);
        return 0;
    }
    else{
        if(this->running_task != -1){
            this->add_done_tasks(this->running_task);
            this->running_task = -1;
            this->set_status(0);
        }
        else{
            cerr << "Warning: the server " << this->get_name() << " is already idle." << endl;
        }
    }
    return 0;
}

void Server::add_done_tasks(int a, bool overwrite){
    this->done_tasks.push_back(a);
    if(overwrite)
        get_task_by_id(a)->set_status(2);
}

void Server::set_status(int a){
    status = a;
    vector <string> lines;
    string line;
    int id;
    
    ifstream in("servers.txt", ios::in);
    if(in.is_open()){
        while(getline(in, line)){
            stringstream ll(line);
            ll >> id;
            cout << "line now is: " << line << endl;
            if(id == this->id){
                line = this->get_server_line();
            }
            cout << "line after id check is: " << line << endl;
            line += "\n";
            cout << "\\n is added to the end of the line" << endl;
            lines.push_back(line);
        }
        in.close();
        
        ofstream out("servers.txt", ios::out);
        if(out.is_open()){
            for(int i = 0; i < lines.size(); i++){
                out << lines[i];
            }
            out.close();
        }
        else{
            cerr << "We couldn't change the file server.txt because we couldn't open it to write." << endl;
        }
        
    }
    else{
        cerr << "We couldn't change the file server.txt because we couldn't open it" << endl;
    }
    return;
}

int Server::get_status(){
    return status;
}

string Server::get_name(){
    return name;
}

vector <int> Server::get_done_tasks(){
    return this->done_tasks;
}

int Server::get_running_task(){
    return running_task;
}

string Server::get_server_line(){
    string ret;
    ret = to_string(this->id);
    ret += " ";
    ret += this->get_name();
    ret += " ";
    ret += to_string(this->running_task);
    ret += " ";
    for(int i = 0; i < this->done_tasks.size(); i++){
        ret += to_string(this->done_tasks[i]);
        if(i != this->done_tasks.size() - 1){
            ret += " ";
        }
    }
//    ret += "\n";
    return ret;
}

int Server::get_id(){
    return id;
}

Task* get_task_by_id(int id, map <int, Task*> *tasks){
    static map <int, Task*> *p = NULL;
    if(p == NULL && tasks == NULL){
        cerr << "The tasks pointer in get_task_by_id function doesn't set." << endl;
        return NULL;
    }
    else if(p == NULL && tasks != NULL){
        p = tasks;
    }
    
    if(id == -1)
        return NULL;
    
    if(p != NULL){
        return ((*p)[id]);
    }
    
    return NULL;
}