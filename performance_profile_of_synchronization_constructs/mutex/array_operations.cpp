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


#define READ (0)
#define UPDATE (1)
#define SORT (2)
#define MAX_SIZE (100000)
#define NUM_QUERIES (10000)

using namespace std::literals::chrono_literals;
using namespace std::chrono;

int num_elements;
int num_threads;
int arr[MAX_SIZE];
int num_queries;

std::mutex IO;
std::mutex lk_num_q;
std::mutex lk_arr[MAX_SIZE];


std::random_device rd;
std::mt19937 gen(rd());
/* probability of generating the 
    UPDATE is 0.4,
    READ is 0.4 
    SORT is 0.2
*/
std::discrete_distribution<> d({89, 10, 1}); 

inline int option() {
    return d(gen);
}

inline int generate_random_idx () {
    return (rand() % num_elements);
}

inline int update_func () {
    return (rand() % (1000000000 + 7));
}


void process_queries (int my_thd_idx) {
    int idx;
    int op;
    int read_val;

    while (true) {
        {
            /*  lock_guard is a mutex wrapper that provides a convenient 
                RAII-style mechanism for owning a mutex for the duration of a scoped block.  */
            std::lock_guard<std::mutex> lk(lk_num_q);
            if (num_queries) {
                --num_queries;
            } else return;
        }

        /*process the next query here*/
        op = option();
        switch (op) {
            case UPDATE: {
                idx = generate_random_idx();
                lk_arr[idx].lock();
                arr[idx] = update_func();
                lk_arr[idx].unlock();
            } break;
            
            case READ: {
                idx = generate_random_idx();
                lk_arr[idx].lock();
                read_val = arr[idx];
                lk_arr[idx].unlock();
            } break;

            case SORT: {
                    for (int j=0; j < num_elements; ++j){
                        lk_arr[j].lock();
                    }

                    std::sort(arr , arr+num_elements);
                    
                    for (int j=0; j < num_elements; ++j){
                        lk_arr[j].unlock();
                    }
            } break;

            default:
                break;
        }
    }
}

int main() {
    using namespace std;
    srand(0);
    
    auto gen_ld = [&] {return (rand()% INT32_MAX ); };
    generate (begin(arr) , end(arr),  gen_ld);
    
    int tot_exp;
    

    cout << "Enter number of Experiments: " ;
    cin >> tot_exp;
    cout << "Number of Threads: ";
    cin >> num_threads;    
    cout << "Enter Sizes of Array (max "<< MAX_SIZE <<"): " << endl;
    cin >> num_elements;

    for (int exp_num = 0; exp_num < tot_exp; ++exp_num) {
        num_queries = NUM_QUERIES;
        
        vector <thread> thd(num_threads);

        auto start = high_resolution_clock::now();
        
        for (size_t i = 0; i < thd.size(); ++i) {
            thd[i] = thread(process_queries , i);
        }

        for (size_t i = 0; i < thd.size(); ++i) {
            thd[i].join();
        }
        
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start); 
        
        cout.precision(3);
        cout << fixed;
        cout << "Time elapsed: " << (duration.count() / 1e3) << "ms" << endl;
    }

    return 0;
}