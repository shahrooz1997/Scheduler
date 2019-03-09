/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: shahrooz
 *
 * Created on February 28, 2019, 3:20 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Server.h"
#include "Task.h"
#include <vector>
#include <sstream>
#include <map>
#include <cstdio> 
#include <dirent.h>
#include <algorithm>
  

using namespace std;

vector <string> files_in_dir(string addr){
    vector<string> ret;
    
    struct dirent *de;
    
    DIR *dr = opendir(addr.c_str()); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        return ret; 
    } 
  
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
    // for readdir() 
    while ((de = readdir(dr)) != NULL)
        ret.push_back(de->d_name);
//            printf("%s\n", de->d_name); 
    closedir(dr); 
    return ret;
}

int update_servers(map <int, Server*> &servers){
    vector<string> files = files_in_dir("./status/idle");
    
    for(int i = 0; i < files.size(); i++){
        if(files[i] == "." || files[i] == ".."){
            continue;
        }
        for(map<int, Server*>::iterator it = servers.begin(); it != servers.end(); it++){
            if(files[i] == it->second->get_name()){
                it->second->set_running_task(-1);
            }
        }
    }
    return 0;
}

int read_servers(map <int, Server*> &servers){
    vector <int> ids;
    
    ifstream in("servers.txt", ios::in);
    if(!in.is_open()){
        cerr << "Can't open the file servers.txt" << endl;
        return -1;
    }
    string line;
    int temp, id;
    string str_temp;
    
    while(getline(in, line)){
        stringstream ll(line);
        ll >> id;
        ids.push_back(id);
        ll >> str_temp;
        ll >> temp;
        
        if(servers.find(id) != servers.end())
            continue;
        
        servers.insert(pair <int, Server*>(id, new Server(str_temp, id, temp)));

        while(ll >> temp){
            servers[id]->add_done_tasks(temp, false);
        }
        
        if(servers[id]->get_status() == 0){
            string command;
            command = "touch ./status/idle/";
            command += servers[id]->get_name();
            system(command.c_str());
        }
        else{
            string command;
            command = "touch ./status/running/";
            command += servers[id]->get_name();
            system(command.c_str());
        }
    }
    in.close();
    
    for(map <int, Server*>::iterator task_it = servers.begin(); task_it != servers.end(); task_it++){
        if(find(ids.begin(), ids.end(), task_it->first) == ids.end() && task_it->second->get_status() == 0){
            string command;
            command = "rm -rf ";
            command += "./status/idle/";
            command += task_it->second->get_name();
            system(command.c_str());
            
            delete task_it->second;
            servers.erase(task_it);
            task_it--;
        }
    }
    
    return 0;
}

int read_tasks(map <int, Task*> &tasks){
    
    vector <int> ids;
    
    ifstream in("tasks.txt", ios::in);
    if(!in.is_open()){
        cerr << "Can't open the file tasks.txt" << endl;
        return -1;
    }
    string line;
    int temp, id;
    string temp1, temp2, temp3;
    
    while(getline(in, line)){
        stringstream ll(line);
        ll >> id;
        ids.push_back(id);
        ll >> temp1;
        ll >> temp2;
        ll >> temp3;
        
        if(tasks.find(id) != tasks.end())
            continue;
        
        tasks.insert(pair <int, Task*>(id, new Task(temp1, temp2, temp3, id, -1, 0)));
    }
    in.close();
    
    in.open("running_tasks.txt", ios::in);
    if(!in.is_open()){
        cerr << "Can't open the file running_tasks.txt" << endl;
        return -1;
    }
    int server_id;
    
    while(getline(in, line)){
        stringstream ll(line);
        ll >> id;
        ids.push_back(id);
        ll >> temp1;
        ll >> temp2;
        ll >> temp3;
        ll >> server_id;
        
        if(tasks.find(id) != tasks.end())
            continue;
        
        tasks.insert(pair <int, Task*>(id, new Task(temp1, temp2, temp3, id, server_id, 1)));
    }
    in.close();
    
    in.open("done_tasks.txt", ios::in);
    if(!in.is_open()){
        cerr << "Can't open the file done_tasks.txt" << endl;
        return -1;
    }
    
    while(getline(in, line)){
        stringstream ll(line);
        ll >> id;
        ids.push_back(id);
        ll >> temp1;
        ll >> temp2;
        ll >> temp3;
        ll >> server_id;
        
        if(tasks.find(id) != tasks.end())
            continue;
        
        tasks.insert(pair <int, Task*>(id, new Task(temp1, temp2, temp3, id, server_id, 2)));
    }
    in.close();
    
    for(map <int, Task*>::iterator task_it = tasks.begin(); task_it != tasks.end(); task_it++){
        if(find(ids.begin(), ids.end(), task_it->first) == ids.end()){
            delete task_it->second;
            tasks.erase(task_it);
            task_it--;
        }
    }
    
    return 0;
}

int run(Task* task, Server* server){
    string command = "python prepare_for_runnig.py ";
    command += task->get_cache_type() + " " + task->get_benchmark() + " " + task->get_mem_fraction();
    system(command.c_str());
    
    cout << "Transferring" << endl;
    command = "scp ./prepared_files/";
    command += task->get_cache_type() + "_" + task->get_mem_fraction() + ".tar.gz ";
    command += server->get_name() + ":~/auto_zsim/";
    system(command.c_str());
    
    command = "ssh -t ";
    command += server->get_name() + " ";
    command += "tar -xzvf ./auto_zsim/" + task->get_cache_type() + "_" + task->get_mem_fraction() + ".tar.gz -C ./auto_zsim >/dev/null";
    system(command.c_str());
    
    cout << "running the benchmark " << task->get_benchmark() << " on " << task->get_cache_type() << "_" << task->get_mem_fraction() << endl;
    command = "ssh -t ";
    command += server->get_name() + " ";
    command += "\"source ./auto_zsim/prepare.sh; cd ./auto_zsim/" + task->get_cache_type() + "_" + task->get_mem_fraction() + "; ";
    command += "python runner.py " + server->get_name() + " " + task->get_cache_type() + " " + task->get_benchmark() + " " + task->get_mem_fraction();
    command += "\" </dev/null >/dev/null 2>&1 &";
    system(command.c_str());
    
    return 0;
}

int utilize_free_servers(map <int, Server*> &servers, map <int, Task*> &tasks){
    
    for(map <int, Task*>::iterator task_it = tasks.begin(); task_it != tasks.end(); task_it++){
        if(task_it->second->get_status() == 0){
            for(map <int, Server*>::iterator s_it = servers.begin(); s_it != servers.end(); s_it++){
                if(s_it->second->get_status() == 0){
                    run(task_it->second, s_it->second);
                    s_it->second->set_running_task(task_it->first);
                    task_it->second->set_runner_server(s_it->first);
                    break;
                }
            }
        }
    }
    
    return 0;
}

int scheduler(map <int, Server*> &servers, map <int, Task*> &tasks){

    while(true){
        
        update_servers(servers);

        if(read_tasks(tasks) != 0)
            return -1;

        if(read_servers(servers) != 0)
            return -1;

        utilize_free_servers(servers, tasks);
        
        system("sleep 300");
    }
    
    
    return 0;
}

int delete_status_files(){
    vector<string> files = files_in_dir("./status/idle");
    for(int i = 0; i < files.size(); i++){
        if(files[i] == "." || files[i] == ".."){
            continue;
        }
        string command;
        command = "rm -rf ./status/idle/";
        command += files[i];
        system(command.c_str());
    }
    
    files = files_in_dir("./status/running");
    for(int i = 0; i < files.size(); i++){
        if(files[i] == "." || files[i] == ".."){
            continue;
        }
        string command;
        command = "rm -rf ./status/running/";
        command += files[i];
        system(command.c_str());
    }
    
    return 0;
}

/*
 * 
 */
int main(int argc, char** argv) {
    map <int, Server*> servers;
    map <int, Task*> tasks;
    
    get_task_by_id(-1, &tasks); // Must be called so that the function can work.
    
    delete_status_files();
    
    scheduler(servers, tasks);
    
    for(map<int, Server*>::iterator it = servers.begin(); it != servers.end(); it++){
        delete it->second;
    }
    
    for(map<int, Task*>::iterator it = tasks.begin(); it != tasks.end(); it++){
        delete it->second;
    }

    return 0;
}

