/*                               -*- Mode: C -*- 
 * pod.c -- "Pretty OD" make a useful (hex) octal dump...
 * 
 * $Header$
 * 
 * Copyright (C)  1991  The Board of Trustees of The Leland Stanford
 *                                  Junior University.  All Rights Reserved.
 * 
 * Creator         : Tom Pavel  <pavel>     Sun Jul 26 23:32:11 1992
 * Modifier        : Tom Pavel  <pavel>     Sun Jul 26 23:54:59 1992
 * Update Count    : 4
 * Status          : Unknown, Use with caution!
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef lint
static char RCSid[] = "$Header$";
#endif

char* progname;

void Error(char* s)
{
   fprintf(stderr, "%s: %s\n", progname, s);
   exit(1);
}

void Usage()
{
   fprintf(stderr, "Usage: %s [file] \n", progname);
   exit(2);
}


void Dump( FILE* fp)
{
   unsigned char buf[16];
   int nbytes;
   int pos = 0;
   int i;

   while ( nbytes = fread(buf, 1, 16, fp) ) {
      printf( "%8x:  ", pos);
      for (i=0; i < nbytes; i++) {
	 printf("%2.2x ", buf[i]);
      }
      printf(" ");
      for (i=0; i < nbytes; i++) {
	 if (isprint(buf[i]))
	     printf("%c", buf[i]);
	 else
	    printf(".");
      }
      printf("\n");
      pos += nbytes;
   }
}


main( int argc, char** argv)
{
   int c;
   extern int optind;
   extern char* optarg;
   int errflag = 0;
   FILE* fp;

   progname = argv[0];
   while ((c = getopt(argc, argv, "x")) != EOF) {
      switch (c) {
      case 'x':
	 /* xxx option */
	 break;
      default:
	 errflag = 1 ;
	 break;
      }
   }
   if (errflag) {
      Usage();
   }

   if (optind < argc)
      fp = fopen( argv[optind], "r");
   else
      fp = stdin;

   /* Do business here... */
   Dump(fp);

   exit(0);
}
