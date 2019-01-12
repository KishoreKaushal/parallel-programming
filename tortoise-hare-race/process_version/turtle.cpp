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

#define STEP (1)

void handle_signal(int sig){
    switch(sig){
        case SIGUSR1:   /*used by god to signal the start the race*/
                cout<<"Turtle recieved signal from god to run."<<endl; 
                break;
        case SIGUSR2: 
                cout<<"Turtle stopped running.";
                break;
        case SIGTERM:
                cout<<"Turtle recieved race termination request."<<endl;
                exit(0);
                break;
        default: break;
    }
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGTERM, handle_signal);
}

int main(int argc, char *argv[]){
    cout<<"Turtle is here with pid: "<<getpid()<<" parent id: "<<getppid()<<endl;
    sleep(1);

    pid_t mypid = getpid();
    pid_t hpid;
    int turtle2hare_fd, hare2turtle_fd, turtle2god_fd, turtle2reporter_fd;
    int ret;
    proc my('T', 0, 0);
    
    /*read hare pid from the pipe*/
    hare2turtle_fd = open(hare2turtle, O_RDONLY);
    ret = read(hare2turtle_fd, &hpid, sizeof(int));
    close(hare2turtle_fd);
    cout<<"Hare PID read by turtle: "<< hpid <<endl;

    /*send mypid to hare*/
    turtle2hare_fd = open(turtle2hare, O_WRONLY);
    ret = write(turtle2hare_fd, &mypid, sizeof(int));
    close(turtle2hare_fd);

    /*signal the god for the race begin request*/
    turtle2god_fd = open(turtle2god, O_WRONLY);
    ret = write(turtle2god_fd, &mypid, sizeof(int));
    close(turtle2god_fd);


    /*pause for the God signal to start the race.*/
    pause();




    return 0;
}