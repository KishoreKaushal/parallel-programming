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
int arr[MAX_SIZE];


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

int main() {
    using namespace std;
    srand(0);

    auto gen_ld = [&] {return (rand()% INT32_MAX ); };
    generate (begin(arr) , end(arr),  gen_ld);
    
    int tot_exp;
    

    cout << "Enter number of Experiments: " ;
    cin >> tot_exp;
    
    cout << "Enter Sizes of Array (max "<< MAX_SIZE <<"): " << endl;
    cin >> num_elements;

    for (int exp_num = 0; exp_num < tot_exp; ++exp_num) {
        int num_queries = NUM_QUERIES;
        int op;
        int idx;
        int read_val;

        auto start = high_resolution_clock::now();
        
        while (num_queries--) {
            op = option();
            switch (op)
            {
                case UPDATE: 
                    idx = generate_random_idx();
                    arr[idx] = update_func();
                    break;
                
                case READ:
                    idx = generate_random_idx();
                    read_val = arr[idx];
                    break;

                case SORT:
                    sort(arr , arr+num_elements);
                    break;

                default:
                    break;
            }
        }

        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start); 
        
        cout.precision(3);
        cout << fixed;
        cout << "Time elapsed: " << (duration.count() / 1e3) << "ms" << endl;
    }

    return 0;
}