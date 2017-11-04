/*                               -*- Mode: C -*- 
 * hexvert.c -- Convert deciman and hex numbers
 * 
 * $Id$
 * 
 * Copyright (C)  1994  The Board of Trustees of The Leland Stanford
 *                                  Junior University.  All Rights Reserved.
 * 
 * Creator         : Tom Pavel  <pavel>     Sat Jan 22 15:31:59 1994
 * Modifier        : Tom Pavel  <pavel>     Sat Jan 22 15:54:07 1994
 * Update Count    : 5
 * Status          : Unknown, Use with caution!
 */


#include <stdio.h>
#include <stdlib.h>

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
   fprintf(stderr, "Usage: %s [<number>]\n", progname);
   exit(2);
}


void convert( const char* str);


main( int argc, char** argv)
{
   progname = argv[0];

   if (argc > 2) {
      Usage();
   } else if (argc == 2) {
      convert(argv[1]);
      exit(0);
   } else {
      char line[1024];
      while (fgets(line, sizeof(line), stdin)) {
	 convert(line);
      }
      exit(0);
   }
   

}

void convert( const char* str)
{
   int num;

   if (sscanf(str, "0x%x", &num) > 0) {
      printf("%d\n", num);
   } else if (sscanf(str, "%d", &num) > 0) {
      printf("0x%x\n", num);
   } else {
      char buf[1024];
      sprintf(buf, "Unknown number: %s", str);
      Error(buf);
   }
}

