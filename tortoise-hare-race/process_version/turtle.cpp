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


void handle_signal(int sig){
    switch(sig){
        case SIGUSR1:   /*used by god to signal the start the race*/
                cout<<"Turtle recieved signal from god to run."<<endl; 
                break;
        case SIGUSR2:   /*used by reporter and god*/
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
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGTERM, handle_signal);

    pid_t mypid = getpid();
    pid_t hpid;
    int turtle2hare_fd, hare2turtle_fd, turtle2god_fd, turtle2reporter_fd, god2turtle_fd;
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

    while (true && my.pos < FINAL_POS) {
        /*take a step*/
        my.pos += TSTEP;
        my.t += 1;

        /*send mypid to hare*/
        turtle2hare_fd = open(turtle2hare, O_WRONLY);
        ret = write(turtle2hare_fd, &my.pos, sizeof(int));
        close(turtle2hare_fd);


        pause();    /*recieve signal from reporter*/

        /*write to reporter*/
        turtle2reporter_fd = open(turtle2reporter, O_WRONLY);
        ret = write(turtle2reporter_fd, &my, sizeof(proc));
        close(turtle2reporter_fd);

        /*for reposition by god*/
        pause();
        kill(getppid(), SIGUSR2);
        god2turtle_fd = open(god2turtle, O_RDONLY);
        read(god2turtle_fd, &ret, sizeof(int));
        close(god2turtle_fd);

        if (ret >= 0){
            my.pos = ret;
            cout<<"Turtle repositioned at "<<my.pos<<endl;
        }

        /*turtle will signal reporter*/
        turtle2reporter_fd = open(turtle2reporter, O_WRONLY);
        ret = write(turtle2reporter_fd, &my, sizeof(proc));
        close(turtle2reporter_fd);
    }

    return 0;
}