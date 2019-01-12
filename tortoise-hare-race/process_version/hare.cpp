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

#define STEP (3) 

void handle_signal(int sig){
    switch(sig){
        case SIGUSR1:
                cout<<"Hare recieved Signal from reporter to run."<<endl; 
                break;
        case SIGUSR2: 
                cout<<"Hare stopped running.";
                break;
        case SIGTERM:
                cout<<"Hare recieved race termination request."<<endl;
                exit(0);
                break;
        default: break;
    }
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGTERM, handle_signal);
}

int main(int argc, char *argv[]){

    return 0;
}