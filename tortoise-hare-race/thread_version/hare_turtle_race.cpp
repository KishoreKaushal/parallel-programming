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

#include <bits/stdc++.h>
#include <thread>

#define HSTEP (3)
#define TSTEP (1)
#define DELTA (10)
#define FINAL_POS (100)
#define MAX_SLEEP_TIME (30)
#define REPOSITIONING_PROBABILITY (0.02)

using namespace std;
using namespace std::literals::chrono_literals;

int t = 0;                          /* clock */
int hare_pos = 0;                   /* hare current position */
int turtle_pos = 0;                 /* turtle current position */
int hare_sleeping_duration = 0;

bool reposition_lck = false;
bool repositioned_hare = false;
bool repositioned_turtle = false;
bool race_finished = false;

bool hare_ran = false;             /* true: hare ran one step */
bool turtle_ran = false;           /* true: turtle ran one step */
bool hare_sleeping = false;         /* true: hare is sleeping */



void hare(){
    cout << "Hare thread id: " << this_thread::get_id()<<endl;
    
    while (not race_finished) {
        this_thread::sleep_for(1s);
        if (not hare_ran and not hare_sleeping) {
            hare_pos += HSTEP;
            hare_ran = true;
        }
    }

}

void turtle(){
    cout << "Turtle thread id: " << this_thread::get_id() << endl;
    while (not race_finished) {
        this_thread::sleep_for(1s);
        if (not turtle_ran) {
            turtle_pos += TSTEP;
            turtle_ran = true;
        }
    }
}

void god(){
    cout << "God thread id: "<< this_thread::get_id() << endl;
    
    double p;
    int tmp1, tmp2;

    while (not race_finished) {
        this_thread::sleep_for(1s);
        /*repositioning with probability*/
        p = ( rand() % 100 ) / 100.0;
        
        if (p <= REPOSITIONING_PROBABILITY) {
            reposition_lck = true;
            this_thread::sleep_for(1s);

            printf("Before reposition t = %d, \thpos = %d, \ttpos = %d\n", t, hare_pos, turtle_pos);

            printf("To reposition hare enter a value between 0 and %d  or -1 to cancel reposition: ", FINAL_POS);

            cin >> tmp1;

            if (tmp1 > -1) hare_pos = tmp1;

            printf("To reposition turtle enter a value between 0 and %d  or -1 to cancel reposition: ", FINAL_POS);
            cin >> tmp2;

            if (tmp2 > -1) turtle_pos = tmp2;

            if (tmp1>-1 and tmp2 > -1)
                printf("After reposition t = %d, \thpos = %d, \ttpos = %d\n", t, hare_pos, turtle_pos);
            
            reposition_lck = false;
        }
    }
}

void reporter(){
    cout << "Reporter thread id: "<< this_thread::get_id() << endl;
    while (not race_finished) {
        this_thread::sleep_for(1s);
        //printf("hareRan: %d , hare_sleeping: %d , turtleRan: %d, repositionLck: %d\n", hare_ran, hare_sleeping, turtle_ran, reposition_lck);
        if ((hare_ran or hare_sleeping) and turtle_ran and not reposition_lck) {
        
            // this_thread::sleep_for(1s);
            t += 1;     /* next timestep */
            printf("Reporting at t = %d, \thpos = %d, \ttpos = %d\n", t, hare_pos, turtle_pos);

            if (hare_pos >= FINAL_POS and turtle_pos < FINAL_POS){
                cout<<"Hare wins the race."<<endl;
                race_finished = true;
                break;
            } else if (hare_pos < FINAL_POS and turtle_pos >= FINAL_POS) {
                cout<<"Turtle wins the race."<<endl;
                race_finished = true;
                break;
            } else if (hare_pos >= FINAL_POS and turtle_pos >= FINAL_POS) {
                cout<<"Match Draw."<<endl;
                race_finished = true;
                break;
            }

            if (hare_sleeping and (--hare_sleeping_duration <= 0)) {
                cout << "Hare woke up from a long night." << endl;
                hare_sleeping = false;
            }

            if ((hare_pos - turtle_pos >= DELTA) and (not hare_sleeping)) {
                hare_sleeping_duration = rand() % MAX_SLEEP_TIME + 1;
                
                cout << "\nHare says, 'It's a long night, I am going to sleep for t="
                     << hare_sleeping_duration << "'." << endl;
                
                hare_sleeping = true;
            }

            hare_ran = turtle_ran = false;    /* enabling them to run for next_timestep */
        }
    }
}

int main(){
    cout<<"\nStarting thread version\n"<<endl;
    srand(time(NULL));

    /*Firing up the threads*/
    thread hare_t(hare);
    thread turtle_t(turtle);
    thread god_t(god);
    thread reporter_t(reporter);


    /*Waiting for threads*/
    hare_t.join();
    turtle_t.join();
    god_t.join();
    reporter_t.join();

    cout << "Race finished."<<endl;
    return 0;
}