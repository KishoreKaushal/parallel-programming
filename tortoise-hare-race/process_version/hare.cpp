#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h> 
#include <limits.h>
#include "udef.hpp"

using namespace std;


void handle_signal(int sig){
    switch(sig){
        case SIGUSR1:   /*used by god to signal the start the race*/
                cout<<"Hare recieved signal from god to run."<<endl; 
                break;
        case SIGUSR2:   /*used by reporter*/
                // cout<<"Continuing Hare.";
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
    cout<<"Hare is here with pid: "<<getpid()<<" parent id: "<<getppid()<<endl;
    sleep(1);
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGTERM, handle_signal);
    
    pid_t mypid=getpid();
    pid_t tpid;
    int turtle2hare_fd, hare2turtle_fd, hare2god_fd, hare2reporter_fd;
    int ret;
    proc my('H', 0, 0);

    /*send mypid to turtle*/
    hare2turtle_fd = open(hare2turtle, O_WRONLY);
    ret = write(hare2turtle_fd, &mypid, sizeof(int));
    close(hare2turtle_fd);

    /*read turtle pid from the pipe*/
    turtle2hare_fd = open(turtle2hare, O_RDONLY);
    ret = read(turtle2hare_fd, &tpid, sizeof(int));
    close(turtle2hare_fd);
    cout<<"Turtle PID read by hare: "<< tpid <<endl;

    /*writing to god's pipe*/
    hare2god_fd = open(hare2god, O_WRONLY);
    ret = write(hare2god_fd, &mypid, sizeof(int));
    close(hare2god_fd);
    

    /*pause for the God signal to start the race.*/
    pause();

    while (true && my.pos < FINAL_POS){
        /*take a step*/
        my.pos += HSTEP;
        my.t += 1;


        /*recieve signal from reporter to write the info to reporter pipe and proceed one time step*/
        pause();

        /*write to the reporter pipe*/
        hare2reporter_fd = open(hare2reporter, O_WRONLY);
        ret = write(hare2reporter_fd, &my, sizeof(proc));
        close(hare2reporter_fd);

    }
    return 0;
}