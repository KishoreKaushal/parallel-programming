#include <iostream>
using namespace std;

#ifndef UDEF_INCLUDE
#define UDEF_INCLUDE

#define PERM (0666)

#define GOD (0)
#define HARE (1)
#define TURTLE (2)
#define REPORTER (3)

#define TOTAL_PROC (4) 

#define FINAL_POS (100)

#define HSTEP (3)
#define TSTEP (1)

char program[][30] = {"./god.out", "./hare.out", "./turtle.out", "./reporter.out"};

const char hare2turtle[] = "/tmp/hare2turtle";
const char turtle2hare[] = "/tmp/turtle2hare";

const char hare2reporter[] = "/tmp/hare2reporter";
const char turtle2reporter[] = "/tmp/turtle2reporter";

const char god2hare[] = "/tmp/god2hare";
const char god2turtle[] = "/tmp/god2turtle";

const char hare2god[] = "/tmp/hare2god";
const char turtle2god[] = "/tmp/turtle2god";

void mkfifo(){
    mkfifo(hare2turtle,     PERM);
    mkfifo(turtle2hare,     PERM);
    mkfifo(hare2reporter,   PERM);
    mkfifo(turtle2reporter, PERM);
    mkfifo(god2hare,        PERM);
    mkfifo(god2turtle,      PERM);
    mkfifo(hare2god,        PERM);
    mkfifo(turtle2god,      PERM);
}

void unlink(){
    unlink(hare2turtle);
    unlink(turtle2hare);
    unlink(hare2reporter);
    unlink(turtle2reporter);
    unlink(god2hare);
    unlink(god2turtle);
    unlink(hare2god);
    unlink(turtle2god);
}


struct proc {
    char name;
    int pos;
    int t;
    proc(char _name='R', int _pos=0, int _t=0){
        name = _name;
        pos = _pos;
        t = _t;
    }
};

#endif