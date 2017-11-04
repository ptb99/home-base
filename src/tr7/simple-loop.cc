/*                               -*- Mode: C -*- 
 * simple-loop.cc -- simple integer/cycle benchmark
 *
 * $Id$
 *
 * Creator         : Tom Pavel  <tom>     Wed Dec 14 15:00 PST 2011
 * Modifier        : Tom Pavel  <tom>     Wed Dec 14 17:16:05 PST 2011
 * Update Count    : 3
 * Status          : working
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef lint
static char RCSid[] = "$Header$";
#endif 


class Timer {
public:
    Timer() {
	_start.tv_sec = _start.tv_nsec = 0;
	_stop.tv_sec = _stop.tv_nsec = 0;
    }
    ~Timer() { }

    void Start() { clock_gettime (CLOCK_REALTIME, &_start); }
    void Stop() { clock_gettime (CLOCK_REALTIME, &_stop); }

    double getStart() { return _start.tv_sec + _start.tv_nsec*1e-9; }
    double getStop() { return _stop.tv_sec + _stop.tv_nsec*1e-9; }
    double getDiff() { return (_stop.tv_sec - _start.tv_sec) +
	    (_stop.tv_nsec - _start.tv_nsec) * 1e-9; }

private:
    struct timespec _start;
    struct timespec _stop;
};



void
Usage (const char* progname)
{
   (void) fprintf (stderr,
		   "usage: %s <num-loop> <array-size>\n",
		   progname);
   exit (EXIT_FAILURE);
}



int
main (int argc, const char** argv)
{
    const int MULT = 1000000;

    if (argc != 3) {
	Usage (argv[0]);
	return 1;		// Usage() doesn't return
    }

    int N = atoi (argv[1]);
    int size = atoi (argv[2]);
    if (N <= 0 || size <= 0) {
	Usage (argv[0]);
	return 1;		// Usage() doesn't return
    }

    // init an array
    int* x = new int[size];
    for (int i=0; i < size; ++i) {
	x[i] = i % 199;
    }

    Timer t;
    t.Start();

    int sum = 0;
    int kmax = MULT/size;
    if (kmax < 1) kmax = 1;
    for (int k=0; k < kmax; ++k) {
	// effectively multiply the given j by kmax

	if (size == 1) {

	    // special case to remove the loop for array of 1
	    for (int j=0; j < N; ++j) {
		sum += (*x % 7) ? j : 42;
	    }

	} else {

	    for (int j=0; j < N; ++j) {
		for (int i=0; i < size; ++i) {
		    // silly arithmetic operation to look like we're busy...
		    sum += (x[i] % 7) ? j : x[i];
		}
	    }

	}
    }

    t.Stop();

    // free our array (unnecessary)
    delete [] x;

    // print out results
    printf ("[ignore useless result = %d]\n", sum);
    double dt = t.getDiff();
    printf ("%d k iterations of %d array in %f sec = %f nsec/op\n",
	    N*kmax/1000, size, dt, dt/N/kmax/size*1e9);

    return 0;
}
