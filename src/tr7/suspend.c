#include <err.h>
#include <pthread.h>
#include <pthread_np.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define SLEEP	1000000		/* 1 second */

pthread_t bg1;
pthread_t bg2;

static void *
bgfunc(void *arg)
{
    char ch = *(char *)arg;

    for ( ; ; ) {
	putchar(ch);
	usleep(SLEEP);
    }
}

int
main(int argc, char **argv)
{
    struct termios t;
    int ret;

    /* Set up stdin and stdout to deliver characters immediately. */
    setbuf(stdout, NULL);
    tcgetattr(fileno(stdin), &t);
    t.c_lflag &= ~(ECHO | ICANON);
    t.c_cc[VMIN] = 1;
    t.c_cc[VTIME] = 0;
    tcsetattr(fileno(stdin), TCSAFLUSH, &t);

    /* Start two background threads. */
    pthread_create(&bg1, NULL, bgfunc, "1");
    usleep(SLEEP / 2);
    pthread_create(&bg2, NULL, bgfunc, "2");

    /*
     * On each keystroke, toggle background thread 1 between suspended
     * and running.
     */
    for ( ; ; ) {
	getchar();
	fputs("-S-", stdout);
	if ((ret = pthread_suspend_np(bg1)) != 0)
	    errc(1, ret, "pthread_suspend_np failed");
	getchar();
	fputs("-R-", stdout);
	if ((ret = pthread_resume_np(bg1)) != 0)
	    errc(1, ret, "pthread_resume_np failed");
    }
    return 0;
}
