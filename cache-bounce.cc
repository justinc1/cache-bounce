// http://arighi.blogspot.si/2008/12/cacheline-bouncing.html
// https://developers.redhat.com/blog/2014/03/10/determining-whether-an-application-has-poor-cache-performance-2/

#include <iostream>
#include <thread>
#include <cstdint>
#include <unistd.h>
#include <string.h>

#define __cacheline_aligned__ __attribute__((__aligned__(CACHELINE_SIZE)))

const int counter_len = 320;
struct Counter {
    uint64_t value[counter_len];
};
static Counter counter __cacheline_aligned__;

const int num_iterations = 1000*1000*1000;
const int pos0 = 0;
//const int pos1 = 1;
const int pos1 = CACHELINE_SIZE/sizeof(counter.value[0]);
static_assert(pos1 < counter_len);

static void do_increment(int id, int pos, Counter* cnt) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(id, &cpuset);
    //int rc = pthread_setaffinity_np(pthread_self(),
    //                                sizeof(cpu_set_t), &cpuset);
    std::cout << "id=" << id << " Hello, World" << std::endl;
    for (int ii=0; ii<num_iterations; ii++) {
        cnt->value[pos]++;
    }
    std::cout << "id=" << id << " Done" << std::endl;
}

int main() {
    memset(&counter, 0x00, sizeof(counter));
    
    std::thread t0(do_increment, 0, pos0, &counter);
    usleep(1);
    std::thread t1(do_increment, 1, pos1, &counter);

    t0.join();
    t1.join();
    return 0;
}
