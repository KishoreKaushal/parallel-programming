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

    /*------------------The God program goes here--------------------*/
    
    int hare2god_fd, turtle2god_fd;
    int ret, rcvh, rcvt;

    /*read pipe signal from hare*/
    hare2god_fd = open(hare2god, O_RDONLY);
    ret = read(hare2god_fd, &rcvh, sizeof(int));
    close(hare2god_fd);
    
    /*read pipe signal from turtle*/
    turtle2god_fd = open(turtle2god, O_RDONLY);
    ret = read(turtle2god_fd, &rcvt, sizeof(int));
    close(turtle2god_fd);
    
    if (rcvh != (int)pid[HARE] || rcvt != (int)pid[TURTLE]){
        cout << "Problem while synchronizing the processes." << endl;
    } else {
        cout << "First phase of sync. is working correctly." << endl;
        
        /*signal hare and turtle to start the race*/
        kill(pid[HARE], SIGUSR1);
        kill(pid[TURTLE], SIGUSR2);

        int hare_pos = 0;
        int turtle_pos = 0;
    }



    /*---------------------------------------------------------------*/

    sleep(2);
    for (int i=1; i<TOTAL_PROC; ++i){
        waitpid(pid[i], NULL, 0);
    }
    cout<<"Unlinking all the fifo's"<<endl;
    unlink();   /*Defined in udef.hpp*/
    return 0;
}