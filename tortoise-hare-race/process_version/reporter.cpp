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

bool exit_stat = false;

void handle_signal(int sig){
    switch(sig){
        case SIGUSR1:   /*for god usage*/ 
                break;
        case SIGUSR2: 
                break;
        case SIGTERM:   
                if (exit_stat) exit(0);
                exit_stat = true;
                kill(getppid(), SIGTERM);
                break;
        default: break;
    }
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGTERM, handle_signal);
}

void print_current_status(const proc &hare, const proc &turtle){
        cout << "t = "<<hare.t<<",\thpos = "<<hare.pos<<",\ttpos = "<<turtle.pos<<endl;
}


int main(int argc, char *argv[]){
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGTERM, handle_signal);

    cout<<"Reporter is here with pid: "<<getpid()<<" parent id: "<<getppid()<<endl;
    sleep(1);
    
    proc hare, turtle;

    int turtle2reporter_fd, hare2reporter_fd;
    int ret;
    pid_t hpid, tpid;

    hare2reporter_fd = open(hare2reporter, O_RDONLY);
    ret = read(hare2reporter_fd, &hpid, sizeof(pid_t));
    close(hare2reporter_fd);

    turtle2reporter_fd = open(turtle2reporter, O_RDONLY);
    ret = read(turtle2reporter_fd, &tpid, sizeof(pid_t));
    close(turtle2reporter_fd);

    cout<<"Reporter get hare pid: "<<hpid<<endl;
    cout<<"Reporter get turtle pid: "<<tpid<<endl;

    while (true){
        sleep(1);
        /*read hare status*/
        kill(hpid, SIGUSR2);
        hare2reporter_fd = open(hare2reporter, O_RDONLY);
        ret = read(hare2reporter_fd, &hare, sizeof(proc));
        close(hare2reporter_fd);

        kill(tpid, SIGUSR2);
        /*read turtle status*/
        turtle2reporter_fd = open(turtle2reporter, O_RDONLY);
        ret = read(turtle2reporter_fd, &turtle, sizeof(proc));
        close(turtle2reporter_fd);

        /*print the status info*/
        print_current_status(hare, turtle);

        if (hare.pos >= FINAL_POS && turtle.pos < FINAL_POS){
            cout<<"Hare wins the race."<<endl;
            break;
        } else if (hare.pos < FINAL_POS && turtle.pos >= FINAL_POS) {
            cout<<"Turtle wins the race."<<endl;
            break;
        } else if (hare.pos == FINAL_POS && turtle.pos == FINAL_POS) {
            cout<<"Match Draw."<<endl;
            break;
        }

        /*turtle will signal here to continue*/
        turtle2reporter_fd = open(turtle2reporter, O_RDONLY);
        ret = read(turtle2reporter_fd, &turtle, sizeof(proc));
        close(turtle2reporter_fd);
    }

    kill(hpid, SIGTERM);
    kill(tpid, SIGTERM);
    kill(getppid(), SIGTERM);

    return 0;
}