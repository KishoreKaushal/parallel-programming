#include<iostream>
using namespace std;

#ifndef UDEF_INCLUDE
#define UDEF_INCLUDE

#define PERM (0666)

#define GOD (0)
#define HARE (1)
#define TURTLE (2)
#define REPORTER (3)

#define TOTAL_PROC (4) 

char program[][30] = {"./god.out", "./hare.out", "./turtle.out", "./reporter.out"};

const char hare2turtle[] = "/tmp/hare2turtle";
const char turtle2hare[] = "/tmp/turtle2hare";

const char hare2reporter[] = "/tmp/hare2reporter";
const char turtle2reporter[] = "/tmp/turtle2reporter";

const char god2hare[] = "/tmp/god2hare";
const char god2turtle[] = "/tmp/god2turtle";

void mkfifo(){
    mkfifo(hare2turtle,     PERM);
    mkfifo(turtle2hare,     PERM);
    mkfifo(hare2reporter,   PERM);
    mkfifo(turtle2reporter, PERM);
    mkfifo(god2hare,        PERM);
    mkfifo(god2turtle,      PERM);
}

void unlink(){
    unlink(hare2turtle);
    unlink(turtle2hare);
    unlink(hare2reporter);
    unlink(turtle2reporter);
    unlink(god2hare);
    unlink(god2turtle);
}


#endif