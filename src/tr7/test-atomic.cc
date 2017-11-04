// 
// test program to time atomic data access
//

#include <iostream>
using namespace std;

#include <sys/types.h>
#include <time.h>		// for clock_gettime()
#ifdef USE_ATOMIC
#include <machine/atomic.h>
#endif


const  int N = 10*1024*1024;
static unsigned int array[N];

int
main (int argc, const char* argv[])
{
    struct timespec start;
    clock_gettime (CLOCK_REALTIME, &start);

    for (int i=0; i < N; ++i) {
#ifdef USE_ATOMIC
	atomic_add_int(&array[i], i);
#else
	array[i] += i;
#endif
    }

    struct timespec stop;
    clock_gettime (CLOCK_REALTIME, &stop);

    double dt = stop.tv_sec - start.tv_sec;
    dt += 1.0e-9 * (stop.tv_nsec - start.tv_nsec);

    cout << N << " iter in " << dt << " sec = "
	 << (dt/N)*1e9 << " ns/call"
	 << endl;
}
