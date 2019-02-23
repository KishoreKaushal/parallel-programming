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

int flag = 0;       /* status flag for busy-waiting*/
int num_threads, num_elements;
long double global_sum = 0.0;
long double arr[MAX_SIZE];

std::mutex IO;

void thd_sum (int my_thd_idx, long double (&arr) [MAX_SIZE], int start_idx, int end_idx) {
    #ifdef VERBOSE
    {
        /*  lock_guard is a mutex wrapper that provides a convenient 
            RAII-style mechanism for owning a mutex for the duration of a scoped block.  */
        std::lock_guard<std::mutex> lk(IO);

        std::cerr   << "Thread " << my_thd_idx << " in beginning was executing on core: "
                    << sched_getcpu() << std::endl;
    }
    #endif

    long double local_sum = 0.0;
    for (int i = start_idx; i < end_idx; ++i) {
        local_sum += arr[i];
    }

    while (flag != my_thd_idx) ;    /* busy-wait loop */
    global_sum += local_sum;
    flag = (flag + 1) % num_threads;

    #ifdef VERBOSE
    {
        /*  lock_guard is a mutex wrapper that provides a convenient 
            RAII-style mechanism for owning a mutex for the duration of a scoped block.  */
        std::lock_guard<std::mutex> lk(IO);

        std::cerr   << "Thread " << my_thd_idx << " at the end was executing on core: "
                    << sched_getcpu() << std::endl;
    }
    #endif
}


int main(int argc, char *argv[]){
    using namespace std;
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<long double> dis((numeric_limits<long double>::min() / MAX_SIZE),
                                    (numeric_limits<long double>::max() / MAX_SIZE));

    auto gen_ld = [&] {return (dis(gen)); };
    generate (begin(arr) , end(arr),  gen_ld);

    #ifdef VERBOSE
    cout << "\n==========MUTEX APPROACH==============" << endl;

    cout << "Number of cores configured: " << get_nprocs_conf() << endl
         << "Number of cores available: " << get_nprocs() << endl;
    
    cout << "Hardware Concurrency: "<< thread::hardware_concurrency() << endl;
    #endif

    #ifdef TEST
        num_threads = thread::hardware_concurrency();
        num_elements = MAX_SIZE;
        cout << "num_threads : " << num_threads << endl;
        cout << "num_elements : " << num_elements << endl;
    #else
        cout << "Number of Threads: ";
        cin >> num_threads;
        cout << "Size of Array (max 1000000): " << endl;
        cin >> num_elements;
    #endif

    vector <thread> thd(num_threads);

    cout << "Number of threads spawning: " << thd.size() << endl;

    auto start = high_resolution_clock::now();

    int nxt_idx, num_elements_per_thd = (num_elements/num_threads);
    
    for (size_t i = 0; i < thd.size() - 1; ++i) {
        nxt_idx = num_elements_per_thd * (i+1);
        thd[i] = thread(thd_sum, i, ref(arr), num_elements_per_thd * i, nxt_idx);
    }
    thd[thd.size() - 1] = thread(thd_sum, thd.size()-1, ref(arr),nxt_idx, num_elements);

    for (size_t i = 0; i < thd.size(); ++i) {
        thd[i].join();
    }
    
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start); 
    
    cout.precision(2);
    cout << fixed;
    cout << "Time elapsed: " << (duration.count() / 1e3) << "ms" << endl;
    
    #ifdef VERBOSE
    cout << "\n==========******************************==============\n" << endl;
    #endif

    return 0;
}