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
#include <random>
#include <limits>

#define MAX_SIZE (10000000)

using namespace std::literals::chrono_literals;
using namespace std::chrono;

int num_elements;
long double global_sum = 0.0;
long double arr[MAX_SIZE];


int main(int argc, char *argv[]){
    using namespace std;
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<long double> dis((numeric_limits<long double>::min() / MAX_SIZE),
                                    (numeric_limits<long double>::max() / MAX_SIZE));

    auto gen_ld = [&] {return (dis(gen)); };
    generate (begin(arr) , end(arr),  gen_ld);
    
    int tot_exp;
    
    cout << "Enter number of Experiments: " ;
    cin >> tot_exp;
    
    for (int exp_num = 0; exp_num < tot_exp; ++exp_num) {
        cout << "Enter Sizes of Array (max "<<MAX_SIZE<<"): " << endl;
        cin >> num_elements;

        global_sum = 0.0;
        auto start = high_resolution_clock::now();

        for (int i = 0; i < num_elements; ++i) {
            global_sum += arr[i];
        }
        
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start); 
        
        cout.precision(3);
        cout << fixed;
        cout << "Time elapsed: " << (duration.count() / 1e3) << "ms" << endl;
    }
    
    return 0;
}