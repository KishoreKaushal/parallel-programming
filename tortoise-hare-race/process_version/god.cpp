#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h> 
#include <limits.h>
#include "udef.hpp"

using namespace std;

int main(int argc, char *argv[]){
    mkfifo();   /*Defined in udef.hpp*/
    pid_t pid[TOTAL_PROC];
    
    pid[0] = getpid();
    for (int i=1; i<TOTAL_PROC; ++i){
        if (getpid() == pid[0]){
            pid[i] = fork();
            if (pid[i] == 0){
                char *arg_list[] = {program[i], NULL};
                cout<<"Spawning "<<arg_list[0]<<" with pid: "<<getpid()<<endl;
                execvp(arg_list[0], arg_list);
                break;
            }
        }
    }

    sleep(10);
    for (int i=1; i<TOTAL_PROC; ++i){
        waitpid(pid[i], NULL, 0);
    }
    cout<<"Unlinking all the fifo's"<<endl;
    unlink();   /*Defined in udef.hpp*/
    return 0;
}