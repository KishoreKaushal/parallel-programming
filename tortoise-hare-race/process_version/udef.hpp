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
#define DELTA (10)
#define MAX_SLEEP_TIME (20)

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