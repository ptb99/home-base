/*                               -*- Mode: C -*- 
 * xfontinfo.c -- query X server for font metrics
 * 
 * $Id$
 * 
 * Copyright (C) 2005  Network Physics Inc.  All rights reserved.
 * 
 * Creator         : Tom Pavel  <pavel>     Tue Aug 23 11:54:38 2005
 * Modifier        : Tom Pavel  <pavel>     Tue Aug 23 12:22:43 2005
 * Update Count    : 4
 * Status          : Unknown, Use with caution!
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>             /* for getopt() */



void
Err(const char* progname, const char *fmt, ...)
{
   va_list ap;
   va_start(ap, fmt);

   (void)fprintf(stderr, "%s: ", progname);
   (void)vfprintf(stderr, fmt, ap);
   va_end(ap);
   (void)fprintf(stderr, "\n");
   exit(1);
   /* NOTREACHED */
}



void
Usage (const char* progname)
{
    fprintf (stderr, "usage: %s [ -d display ] font\n", progname);
    exit (1);
}



int 
main (int argc, char** argv)
{
   int ch;
   extern char *optarg;
   extern int optind;

   const char* dname = NULL;
   const char* fname = NULL;
   const char* progname = argv[0];

   Display *dpy;
   int scr;
   XFontStruct *finfo;
   GC gc;
    
   while ((ch = getopt(argc, argv, "d:")) != EOF)
       switch((char)ch) {
       case 'd':
	   dname = optarg;
	   break;
       case '?':
       default:
	   Usage(progname);
       }

   argc -= optind;
   argv += optind;
   if (argc != 1) Usage(progname);

   fname = argv[0];

   dpy = XOpenDisplay(dname);
   if (! dpy)
       Err(progname, "can't open display %s", (dname ? dname : "DEFAULT"));


   finfo = XLoadQueryFont (dpy, fname);
   if (finfo == NULL) 
       Err(progname, "failed to load font |%s|\n", fname);
       
   printf ("\n");
   printf ("max_bounds a=%d d=%d\n",
	   finfo->max_bounds.ascent,finfo->max_bounds.descent);
   printf ("ascent=%d descent=%d\n", finfo->ascent, finfo->descent);
   printf ("\n");

   XFreeFont (dpy, finfo);
}
