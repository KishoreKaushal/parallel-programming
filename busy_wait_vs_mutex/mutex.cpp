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

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <cmath>
#include <vector>
#include <sys/sysinfo.h>
#include <sys/syscall.h>
#include <sched.h>
#include <algorithm>

#define SIZE (10000000)
#define NUM_OPERATIONS (1000000)

using namespace std;
using namespace std::literals::chrono_literals;
using namespace std::chrono;

std::mutex critical_section; 

inline int generate_random_index () {
    return rand() % SIZE;
}

int32_t arr[SIZE];  /* 40 megabytes */


void benchmark(int thd_idx) {
    printf("Thread %d in beginning was executing on core: %d\n", thd_idx, sched_getcpu());
    cout<<endl;

    /*generate a random index*/
    int num_operations_left = NUM_OPERATIONS;
    
    while (num_operations_left--) {
        int idx = generate_random_index();
        critical_section.lock();
        arr[idx] = arr[idx] ^ 1;
        critical_section.unlock();
    }

    printf("Thread %d at the end was executing on core: %d\n", thd_idx, sched_getcpu());
    cout<<endl;
}


int main(int argc, char *argv[]){
    fill(arr, arr+SIZE, (int32_t)0);

    cout << "\n==========MUTEX APPROACH==============" << endl;

    cout << "Number of cores configured: " << get_nprocs_conf() << endl
         << "Number of cores available: " << get_nprocs() << endl;
    
    cout << "Hardware Concurrency: "<< thread::hardware_concurrency() << endl;

    
    const int num_threads = thread::hardware_concurrency();
    vector <thread> thd(num_threads);

    cout << "Number of threads spawning: " << thd.size() << endl;

    auto start = high_resolution_clock::now();

    for (size_t i = 0; i < thd.size(); ++i) {
        thd[i] = thread(benchmark, i);
    }

    for (size_t i = 0; i < thd.size(); ++i) {
        thd[i].join();
    }
    
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start); 
    
    cout.precision(2);
    cout << fixed;
    cout << "Time elapsed: " << (duration.count() / 1e3) << "ms" << endl;
    
    cout << "\n==========******************************==============\n" << endl;
    return 0;
}