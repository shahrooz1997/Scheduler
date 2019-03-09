/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Task.cpp
 * Author: shahrooz
 * 
 * Created on February 28, 2019, 3:21 PM
 */

#include "Task.h"

Task::Task(string &cache_type, string &benchmark, string &mem_fraction, int id, int runnver_server, int status) {
    this->benchmark = benchmark;
    this->cache_type = cache_type;
    this->mem_fraction = mem_fraction;
    
    this->id = id;
    this->runner_server = runnver_server;
    if(runner_server == -1){
        this->status = 0;
    }
    else{
        this->status = status;
    }
    
    this->name = this->cache_type + "_" + this->benchmark + "_" + this->mem_fraction;
}

Task::~Task() {
}

string Task::get_name(){
    return name;
}

string Task::get_task_line(){
    string ret;
    ret = to_string(id);
    ret += " ";
    ret += this->cache_type;
    ret += " ";
    ret += this->benchmark;
    ret += " ";
    ret += this->mem_fraction;
    if(this->status != 0){
        ret += " ";
        ret += to_string(this->runner_server);
    }
    
//    ret += "\n";
    return ret;
}

int Task::set_runner_server(int a){
    if(runner_server != -1){
        cerr << "This task is running by another server." << endl;
        return -1;
    }
    runner_server = a;
    set_status(1);
    
    return 0;
}


int Task::get_status(){
    return status;
}

int Task::set_status(int a){
    if(this->status == a){
        cerr << "Warning: the status of the task is already " << a << endl;
        return 0;
    }
    if(this->status == 0 && a == 2){
        cerr << "ERR: the task want go to done list from waiting list." << endl;
        return -2;
    }
    if(a < this->status){
        cerr << "ERR: the task want go to back!" << endl;
        return -3;
    }
    
    if(this->status == 0 && a == 1){
        status = a;
        vector <string> lines;
        string line;
        int id;
        bool find_flag = false;
        ifstream in("tasks.txt", ios::in);
        if(in.is_open()){
            while(getline(in, line)){
                stringstream ll(line);
                ll >> id;
                if(id == this->id){
                    find_flag = true;
                    continue;
                }
                line += "\n";
                lines.push_back(line);
            }
            in.close();
            
            if(find_flag == false){
                cerr << "the task was not where it is supposed to be." << endl;
                return -4;
            }

            ofstream out("tasks.txt", ios::out);
            if(out.is_open()){
                for(int i = 0; i < lines.size(); i++){
                    out << lines[i];
                }
                out.close();
            }
            else{
                cerr << "We couldn't change the file tasks.txt because we couldn't open it to write." << endl;
            }
            
            ofstream out2("running_tasks.txt", ios::app);
            if(out2.is_open()){
                out2 << this->get_task_line() << endl;
                out2.close();
            }
            else{
                cerr << "We couldn't change the file running_tasks.txt because we couldn't open it to append." << endl;
            }
        }
        else{
            cerr << "We couldn't change the file tasks.txt because we couldn't open it" << endl;
            return -1;
        }
        return 0;
    }
    else if(this->status == 1 && a == 2){
        status = a;
        vector <string> lines;
        string line;
        int id;
        bool find_flag = false;
        ifstream in("running_tasks.txt", ios::in);
        if(in.is_open()){
            while(getline(in, line)){
                stringstream ll(line);
                ll >> id;
                if(id == this->id){
                    find_flag = true;
                    continue;
                }
                line += "\n";
                lines.push_back(line);
            }
            in.close();
            
            if(find_flag == false){
                cerr << "the task was not where it is supposed to be." << endl;
                return -4;
            }

            ofstream out("running_tasks.txt", ios::out);
            if(out.is_open()){
                for(int i = 0; i < lines.size(); i++){
                    out << lines[i];
                }
                out.close();
            }
            else{
                cerr << "We couldn't change the file running_tasks.txt because we couldn't open it to write." << endl;
            }
            
            ofstream out2("done_tasks.txt", ios::app);
            if(out2.is_open()){
                out2 << this->get_task_line() << endl;
                out2.close();
            }
            else{
                cerr << "We couldn't change the file done_tasks.txt because we couldn't open it to append." << endl;
            }
        }
        else{
            cerr << "We couldn't change the file tasks.txt because we couldn't open it" << endl;
            return -1;
        }
        return 0;
    }
    
    status = a;
    
}

string Task::get_cache_type(){
    return cache_type;
}

string Task::get_benchmark(){
    return benchmark;
}

string Task::get_mem_fraction(){
    return mem_fraction;
}

int Task::get_id(){
    return id;
}
