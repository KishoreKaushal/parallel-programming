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
 * Subject: Homework - Parallel Programming
**/

/*
    Calculate the following series sum using threads:
    4( 1 - 1/3 + 1/5 - 1/7 + ........ )

    Answer:
    The above series sum is equal to pi.
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <cmath>

#define PRECISION (15)

using namespace std;
using namespace std::literals::chrono_literals;

double series_sum = 0;

std::mutex critical_section; 

void compute_sum(int init_num = 1, int sign = 1) {
    const double precision = pow(10, -(PRECISION+1));

    double sum = 0;
    double delta = 0;

    for (int n = 1; (delta = (1.0/(4*(n-1) + init_num))) >= precision; ++n) {
        sum += delta;
    }
    
    critical_section.lock();
    {
        /*critical section*/
        series_sum += 4 * sign * sum;
    }
    critical_section.unlock();
}


int main(){
    cout.precision(PRECISION+1);

    /*Firing up the threads*/
    thread td1(compute_sum, 1, 1);
    thread td2(compute_sum, 3, -1);


    /*Waiting for threads*/
    td1.join();
    td2.join();

    cout<<"Series Sum: "<<series_sum<<endl;

    return 0;
}