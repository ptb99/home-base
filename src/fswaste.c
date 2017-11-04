/*                               -*- Mode: C -*- 
 * fswaste.c -- calculate file system waste
 * 
 * $Id$
 * 
 * Copyright (C) 1995  The Board of Trustees of The Leland Stanford
 *                                  Junior University.  All Rights Reserved.
 * 
 * Creator         : Tom Pavel  <pavel>     Wed Sep 27 14:22:09 1995
 * Modifier        : Tom Pavel  <pavel>     Wed Sep 27 17:39:51 1995
 * Update Count    : 18
 * Status          : Unknown, Use with caution!
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>

#ifndef _POSIX_PATH_MAX
#define _POSIX_PATH_MAX MAXPATHLEN
#endif

/* All this just for one lousy constant (for adding in inode overhead): */
#include <sys/param.h>
#include <sys/time.h>
#include <sys/vnode.h>
#ifdef _AIX
#include <jfs/inode.h>
#elif BSD4_4
#include <ufs/ufs/quota.h>
#include <ufs/ufs/inode.h>
#else
#include <ufs/inode.h>
#endif
static int inode_size = sizeof(struct dinode);


#ifndef lint
static char RCSid[] = "$Header$";
#endif

char* progname;
int debug=0;



void Error(char* s)
{
   fprintf(stderr, "%s: %s\n", progname, s);
   exit(1);
}

void Usage()
{
   fprintf(stderr, "Usage: %s <dirname>\n", progname);
   exit(2);
}

/* structure for counting usage/waste */
struct fsusage {
   off_t size;
   long space;
};

void calc_usage (const char* filename, struct fsusage* use);





main( int argc, char** argv)
{
   int optind = 1;

   progname = argv[0];

   if (optind >= argc) {
      Usage();
   }

   while (optind < argc) {
      char* filename = argv[optind++];
      struct fsusage use = {0,0};

      calc_usage (filename, &use);
      printf ("%s:\t %6ldk, %6ldk used, waste=%7.2f%%\n",
	      filename, (long)use.size/1024, (long)use.space/1024,
	      (1.0 - (float)use.size/use.space) * 100.0);
   }

   exit(0);
}


/* 
 * Recursive descent of FILENAME, adding up the st_size and st_blocks used by
 *  all files contained in subdirectories
 */
void
calc_usage (const char* filename, struct fsusage* use)
{
   struct stat buf;

   if (lstat (filename, &buf) < 0) {
      Error ("failed to stat");
   }

   if (S_ISREG(buf.st_mode)) {
      /* regular files:  add in sizes */
      /* if (buf.st_nlink > 1 && linkchk(&buf)) ... */
      use->size += buf.st_size;
      use->space += buf.st_blocks * 512;
      use->space += inode_size;
      /* XXX: need to count indirect blocks... */

   } else if (S_ISDIR(buf.st_mode)) {
      /* directories: recurse */
      struct dirent* dp;
      DIR* dirp = opendir (filename);
      if (! dirp) {
	 fprintf (stderr, "%s: can't open %s\n", progname, filename);
	 return;
      }

      while (dp = readdir(dirp)) {
	 /* Foreach real entry in dir, recurse: */
	 if (strcmp(dp->d_name,".") && strcmp(dp->d_name,"..")) {
	    char newname[_POSIX_PATH_MAX];
	    strcpy (newname, filename);
	    strcat (newname, "/");
	    strcat (newname, dp->d_name);

	    calc_usage (newname, use);
	 }
      }

      closedir (dirp);

      /* Don't forget to add the size of directory to overhead: */
      use->space += buf.st_blocks * 512;
      use->space += inode_size;

   } else {
      /* CHR, BLK, LNK, SOCK, FIFO */
      /* XXX: does this work? */
      use->size += buf.st_size;
      use->space += buf.st_blocks * buf.st_blksize;
      use->space += inode_size;
   }
}




/*
 * This is all for later, when I get around to dealing with hard links.... 
 */
typedef struct _ID {
        dev_t   dev;
        ino_t   inode;
} ID;

int
linkchk(struct stat *p)
{
        static ID *files;
        static int maxfiles, nfiles;
        ID *fp, *start;
        ino_t ino;
        dev_t dev;

        ino = p->st_ino;
        dev = p->st_dev;
        if ((start = files) != NULL)
                for (fp = start + nfiles - 1; fp >= start; --fp)
                        if (ino == fp->inode && dev == fp->dev)
                                return (1);

        if (nfiles == maxfiles &&
	     (files =
	      realloc((char *)files,
		      (size_t)(sizeof(ID) * (maxfiles += 128)))) == NULL) {
	   Error("can't realloc ID files[]");
	}

        files[nfiles].inode = ino;
        files[nfiles].dev = dev;
        ++nfiles;
        return (0);
}
