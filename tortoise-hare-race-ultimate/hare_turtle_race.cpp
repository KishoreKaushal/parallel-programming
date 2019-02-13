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
 * Subject: Assignment 1 - Parallel Programming
**/

#include <bits/stdc++.h>
#include <mutex>
#include <thread>

#define HSTEP (3)
#define TSTEP (1)
#define DELTA (10)
#define FINAL_POS (100)
#define MAX_SLEEP_TIME (30)
#define REPOSITIONING_PROBABILITY (0.02)

using namespace std;
using namespace std::literals::chrono_literals;

std::mutex reposition_mutex; 
std::mutex hare_update_mutex;
std::mutex turtle_update_mutex;

bool test_mode = false;

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

void hare() {
    while (not race_finished) {
        this_thread::sleep_for(1s);
        if (not hare_sleeping and not hare_ran) {
            hare_update_mutex.lock();
            hare_pos += HSTEP;
            hare_ran = true;
            hare_update_mutex.unlock();
        }
    }
}

void turtle(){
    while (not race_finished) {
        this_thread::sleep_for(1s);
        if (not turtle_ran and not turtle_ran) {
            turtle_update_mutex.lock();
            turtle_pos += TSTEP;
            turtle_ran = true;
            turtle_update_mutex.unlock();
        }
    }
}

void god(){
    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(0.0, 0.99);
    uniform_int_distribution<> distr(-FINAL_POS + 1, FINAL_POS - 1);
    double p;

    while (not race_finished) {
        this_thread::sleep_for(1s);
        /*repositioning with probability*/
        p = dis(gen);

        if (p <= REPOSITIONING_PROBABILITY) {
            reposition_mutex.lock();
            hare_update_mutex.lock();
            turtle_update_mutex.lock();

            std::cout<<"*********GOD IS HERE*********"<<endl;
            this_thread::sleep_for(1s);

            std::cout << "Before repositioning at t = " << t 
                 << "\thpos = " << hare_pos 
                 << "\ttpos = " << turtle_pos <<endl;
            
            int tmp1, tmp2;
            cout << "Enter two integers between 0 and " << FINAL_POS 
                 << " for hare and turtle respectively "
                 <<"or integer less than 0 corresponds to no repositioning for respective animal: ";

            if (test_mode) {
                tmp1 = distr(gen);
                tmp2 = distr(gen);
                cout << "Entered value : " << tmp1 <<" "<< tmp2 << endl;
            } else {
                cin >> tmp1 >> tmp2;
            }
            
            if (tmp1 > 0) hare_pos = tmp1;
            if (tmp2 > 0) hare_pos = tmp2;

            std::cout << "After repositioning at t = " << t 
                << "\thpos = " << hare_pos 
                << "\ttpos = " << turtle_pos <<endl;


            cout<<"****WORK OF GOD ENDS HERE****"<<endl;
            
            reposition_mutex.unlock();
            hare_update_mutex.unlock();
            turtle_update_mutex.unlock();
        }

    }
}

void reporter(){
    while (not race_finished) {
        this_thread::sleep_for(1s);
        reposition_mutex.lock();
        hare_update_mutex.lock();
        turtle_update_mutex.lock();

        if ((hare_ran or hare_sleeping) and turtle_ran) {
            t += 1;
            cout << "Reporting at t = " << t 
                 << "\thpos = "<< hare_pos
                 << "\ttpos = "<< turtle_pos << endl;

            if (hare_pos >= FINAL_POS and turtle_pos < FINAL_POS ) {
                cout << "Hare wins the race." << endl;
                race_finished = true;
            } else if (hare_pos < FINAL_POS and turtle_pos >= FINAL_POS) {
                cout << "Turtle wins the race." << endl;
                race_finished = true;
            } else if (hare_pos >= FINAL_POS and turtle_pos >= FINAL_POS) {
                cout<<"Match Draw."<<endl;
                race_finished = true;
            }

            if (race_finished) {
                reposition_mutex.unlock();
                hare_update_mutex.unlock();
                turtle_update_mutex.unlock();
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

            hare_ran = turtle_ran = false;
        }

        reposition_mutex.unlock();
        hare_update_mutex.unlock();
        turtle_update_mutex.unlock();
    }
}

int main(){

    #ifdef TEST
        test_mode = true;
        cout << "Test Mode is ON"  << endl;
    #endif

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