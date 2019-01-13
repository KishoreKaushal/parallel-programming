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
                cout<<"Hare recieved signal from god to run."<<endl; 
                break;
        case SIGUSR2:   /*used by reporter and god*/
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
    srand (time(NULL));
    cout<<"Hare is here with pid: "<<getpid()<<" parent id: "<<getppid()<<endl;
    sleep(1);
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGTERM, handle_signal);
    
    pid_t mypid=getpid();
    pid_t tpid;
    int turtle2hare_fd, hare2turtle_fd, hare2god_fd, hare2reporter_fd, god2hare_fd;
    int ret, tpos;
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
    
    bool sleeping = false;
    int sleep_time = 0;

    /*pause for the God signal to start the race.*/
    pause();

    while (true && my.pos < FINAL_POS){
        if (sleeping) {
            if(--sleep_time == 0) {
                sleeping = false;
                cout<<"Hare woke up from a long night."<<endl;
            }
        }

        if (not sleeping) { /*take a step*/
            my.pos += HSTEP;
        }
        my.t += 1;

        /*read turtle pid from the pipe*/
        turtle2hare_fd = open(turtle2hare, O_RDONLY);
        ret = read(turtle2hare_fd, &tpos, sizeof(int));
        close(turtle2hare_fd);

        if (not sleeping && (my.pos - tpos) >= DELTA) {
            /*hare is going to sleep*/
            sleep_time = rand() % MAX_SLEEP_TIME + 1;
            sleeping = true;
            cout<<"\nHare says, 'It's a long night, I am going to sleep for t="<<sleep_time<<"'."<<endl;
        }

        /*recieve signal from reporter to write the info to reporter pipe 
            and proceed one time step*/
        pause();

        /*write to the reporter pipe*/
        hare2reporter_fd = open(hare2reporter, O_WRONLY);
        ret = write(hare2reporter_fd, &my, sizeof(proc));
        close(hare2reporter_fd);

        /*for reposition by god*/
        kill(getppid(), SIGUSR1);
        god2hare_fd = open(god2hare, O_RDONLY);
        read(god2hare_fd, &ret, sizeof(int));
        close(god2hare_fd);

        if (ret >= 0){
            my.pos = ret;
            if (not sleeping) cout<<"Hare repositioned at "<<my.pos<<endl;
            else cout<<"Hare sleeping and repositioned at "<<my.pos<<endl;
        }

        kill(tpid, SIGUSR2);    /*send a signal to turtle to continue*/
    }
    return 0;
}