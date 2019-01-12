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


void print_current_status(const proc &hare, const proc &turtle){
    // if (hare.t == turtle.t){
        cout << "t = "<<hare.t<<",\thpos = "<<hare.pos<<",\ttpos = "<<turtle.pos<<endl;
    // }
}


int main(int argc, char *argv[]){
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
    }

    kill(hpid, SIGTERM);
    kill(tpid, SIGTERM);

    return 0;
}