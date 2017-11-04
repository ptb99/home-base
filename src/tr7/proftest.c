#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>

void handler (int sig)
{
    exit(0);
}

int
main ()
{
    int count = 10;

    signal (SIGINT, handler);

    while (count-- > 0) {
	float pi = 4 * atan(1.0);
	printf ("%d: pi = %f\n", count, pi);
	sleep(1);
    }
}
