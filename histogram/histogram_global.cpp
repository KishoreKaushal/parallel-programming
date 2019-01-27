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

#define MAX_VAL (10000)

using namespace std;
using namespace std::literals::chrono_literals;
using namespace std::chrono;

std::mutex critical_section; 

inline int generate_random_int () {
    return rand() % MAX_VAL;
}

void compute_histogram(int thd_idx, int N, vector<long long int> &hist) {
    printf("Thread %d in beginning was executing on core: %d\n", thd_idx, sched_getcpu());
    cout<<endl;

    int num;
    int bin_size = MAX_VAL / hist.size();

    for (int i = 0; i < N; ++i){
        num = generate_random_int();
        critical_section.lock();
        ++hist[num/bin_size];
        critical_section.unlock();
    }
    printf("Thread %d at the end was executing on core: %d\n", thd_idx, sched_getcpu());
    cout<<endl;
}


int main(int argc, char *argv[]){
    cout << "\n==========HISTOGRAM USING GLOBAL APPROACH==============" << endl;

    long long int n;
    int bins;

    if (argc != 3) {
        n = 10000000;
        bins = 200;
    } else {
        n = strtoll(argv[1], NULL , 10);
        bins = atoi(argv[2]);
    }

    cout << "Number of cores configured: " << get_nprocs_conf() << endl
         << "Number of cores available: " << get_nprocs() << endl;
    
    cout << "Hardware Concurrency: "<< thread::hardware_concurrency() << endl;

    cout << "Number of integers: " << n << endl
         << "Number of bins: " << bins << endl;

    const int num_threads = thread::hardware_concurrency();
    vector <thread> thd(num_threads);
    vector <long long int> hist(bins,0);

    cout << "Number of threads spawning: " << thd.size() << endl;

    auto start = high_resolution_clock::now();

    for (size_t i = 0; i < thd.size(); ++i) {
        thd[i] = thread(compute_histogram, i, (n+i)/4, ref(hist));
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