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
    cout<<"Reporter is here with pid: "<<getpid()<<" parent id: "<<getppid()<<endl;
    sleep(1);

    while (true){
        
    }

    return 0;
}