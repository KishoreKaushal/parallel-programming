/*
 *   MIT License
 *
 *   Copyright (c) 2019 Kaushal Kishore
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 * **********************************************************************************
 * Author: Kaushal Kishore
 * RollNo.: 111601008
 * Subject: Assignment 0 - Parallel Programming
**/

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

pid_t pid[TOTAL_PROC];

void reposition(int entity){
    string s = entity==HARE?"hare":"turtle";
    int fd;
    if(entity == HARE){
        fd = open(god2hare, O_WRONLY);
    } else {
        fd = open(god2turtle, O_WRONLY);
    }

    cout<<"To reposition "<<s<<" enter a value between 0 and "<<FINAL_POS
        <<" or -1 to cancel reposition: "<<endl;
    int pos=-1;
    cin>>pos;

    write(fd, &pos, sizeof(int));
    close(fd);
}

void handle_signal(int sig){
    switch(sig){
        case SIGUSR1: /*to handle hare reposition input */
                reposition(HARE);
                break;
        case SIGUSR2: /* to handle turtle reposition input */
                reposition(TURTLE);
                break;
        case SIGTERM:
                cout<<"God recieved race termination request."<<endl;
                cout<<"Unlinking all the fifo's"<<endl;

                kill(pid[REPORTER], SIGTERM);

                unlink();   /*Defined in udef.hpp*/
                exit(0);
                break;
        default: break;
    }
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGTERM, handle_signal);
}


int main(int argc, char *argv[]){
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGTERM, handle_signal);

    mkfifo();   /*Defined in udef.hpp*/
    
    
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
        kill(pid[TURTLE], SIGUSR1);

        /*send pids of hare and turtle to reporter*/
        int hare2reporter_fd = open(hare2reporter, O_WRONLY);
        write(hare2reporter_fd, &pid[HARE], sizeof(pid_t));
        close(hare2reporter_fd);
        int turtle2reporter_fd = open(turtle2reporter, O_WRONLY);
        write(turtle2reporter_fd, &pid[TURTLE], sizeof(pid_t));
        close(turtle2reporter_fd);

        /**/
        int hare_pos = 0;
        int turtle_pos = 0;
        int t=0;

        while (true) {
            pause();
        }

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