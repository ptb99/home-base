/**********************************/
/*       Copyright (c) yyyy       */
/*   Riverbed Technology, Inc.    */
/*      All Rights Reserved.      */
/**********************************/

/*
 * template.c -- 
 * 
 * $Id$
 * 
 * Creator         : Tom Pavel  <tom>     Thu Sep  8 21:51:12 1994
 * Modifier        : Tom Pavel  <tom>     Thu Sep  8 21:51:29 1994
 * Update Count    : 1
 * Status          : Unknown, Use with caution!
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
		  "usage: %s [-aChjlmrSTuvwx] [-O|o fmt] [-p pid] [-t tty]\n",
		  progname);
   exit(EXIT_FAILURE);
}


main(int argc, char** argv)
{
   extern char *optarg;
   extern int optind;

   int all, ch, flag, fmt, lineno, pid, prtheader, uid, what, xflg;

   progname = argv[0];
   while ((ch = getopt(argc, argv, "abc:")) != EOF)
      switch((char)ch) {
      case 'a':
	 all = 1;
	 break;
      case 'b':
	 rawcpu = 1;
	 break;
      case 'c':
	 memf = optarg;
	 break;
      case '?':
      default:
	 usage();
      }

   argc -= optind;
   argv += optind;
   if (argc > 1) usage();

   exit(EXIT_SUCCESS);
}


