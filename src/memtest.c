/*                               -*- Mode: C -*- 
 * memtest.c -- 
 * 
 * $Id$
 * 
 * Creator         : Tom Pavel  <tom>     Thu Sep  8 21:51:12 1994
 * Modifier        : Tom Pavel  <tom>     Tue Jul 14 00:45:04 1998
 * Update Count    : 13
 * Status          : Unknown, Use with caution!
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#ifndef lint
static char RCSid[] = "$Header$";
#endif 

char* progname;

#include <stdarg.h>

void
err(const char *fmt, ...)
{
   va_list ap;
   va_start(ap, fmt);

   (void)fprintf(stderr, "%s: ", progname);
   (void)vfprintf(stderr, fmt, ap);
   va_end(ap);
   (void)fprintf(stderr, "\n");
   exit(EXIT_FAILURE);
   /* NOTREACHED */
}


usage()
{
   (void) fprintf(stderr,
		  "usage: %s\n",
		  progname);
   exit(EXIT_FAILURE);
}

int
main(int argc, char** argv)
{
    void* endp;
    extern char end;
    size_t size; 

    progname = argv[0];
    if (argc > 1) usage();

    endp = sbrk(0);
    endp = malloc (100*1024*1024); 
    endp = sbrk(0);

    size = endp - (void*)&end;
    printf ("Mem Usage = 0x%x (%.3f MB)\n", size, size/1024.0/1024.0);

    exit(EXIT_SUCCESS);
}


