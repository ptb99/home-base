/*                               -*- Mode: C -*- 
 * exdump.c -- Dump out interesting info from an a.out file
 * 
 * $Id$
 * 
 * Copyright (C) 1994  The Board of Trustees of The Leland Stanford
 *                                  Junior University.  All Rights Reserved.
 * 
 * Creator         : Tom Pavel  <pavel>     Wed Mar 30 16:19:34 1994
 * Modifier        : Tom Pavel  <tom>     Tue Nov 21 01:23:55 1995
 * Update Count    : 44
 * Status          : Unknown, Use with caution!
 */


#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <a.out.h>
/* Actually, nlist is (conflictingly) defined in a.out.h ... */
/* #include <nlist.h> */
#include <link.h>

/* Fixups for FreeBSD: */
#ifndef N_BSSADDR
#define N_BSSADDR(h)    (N_DATADDR(h) + h.a_data)
#endif 
#ifndef N_TRELOFF
#define N_TRELOFF(h)    N_RELOFF(h)
#endif 
#ifndef N_DRELOFF
#define N_DRELOFF(h)    (N_RELOFF(h) + h.a_trsize)
#endif 



#ifndef lint
static char RCSid[] = "$Header$";
#endif

char* progname;



void Error(char* s)
{
   char buf[512];
   sprintf(buf, "%s: %s\n", progname, s);
   perror(buf);
   exit(1);
}

void Usage()
{
   fprintf(stderr, "Usage: %s [-f] [-s]\n", progname);
   fprintf(stderr, "[crib sheet:   ZMAGIC QMAGIC NMAGIC OMAGIC]\n");
   fprintf(stderr, "[non-swapped:  0x010b 0x00cc 0x0108 0x0107]\n");
   fprintf(stderr, "[byte-swapped: 0x0b01 0xcc00 0x0801 0x0701]\n");
   exit(2);
}



/* Auxilliary functions: */
long
byteswap (long in)
{
   long res;
   char* a = (char*)&in;
   char* b = (char*)&res;

   b[0] = a[3];
   b[1] = a[2];
   b[2] = a[1];
   b[3] = a[0];

   return res;
}

unsigned int FindSym(struct nlist* syms,
		     unsigned int sym_len,
		     const char* name)
{
   struct nlist* end = (void*) syms + sym_len;
   char* str_base = (char*) end;
   struct nlist* sp = syms;
   while (sp < end) {
      char* str = str_base + sp->n_un.n_strx;
      if (strcmp(str, name) == 0) {
	 return sp - syms;
      }
      sp++;
   }
   return -1;			/* Not found */
}


/* prototypes for mach-dep functions: */
inline int is_dynamic (struct exec* hdr);
void print_magic (struct exec* hdr);
void print_dynamic (void* p, void* base, void* fp);





main( int argc, char** argv)
{
   int c;
   extern int optind;
   extern char* optarg;
   int errflag = 0;
   int debug = 0;
   int force = 0;
   int swap = 0;
   int print_filename = 1;


   progname = argv[0];
   while ((c = getopt(argc, argv, "dfs")) != EOF) {
      switch (c) {
      case 'f':
	 force = 1;
	 break;
      case 's':
	 swap = 1;
	 break;
      case 'd':
	 debug = 1;
	 break;
      default:
	 errflag = 1 ;
	 break;
      }
   }
   if (errflag || optind >= argc) {
      Usage();
   }

   if (optind == (argc - 1))
      print_filename = 0;

   /* Do business here... */
   while (optind < argc) {
      char* filename = argv[optind++];

      int fd;
      struct exec hdr;
      long midmagic;
      int dynamic;

      fd = open(filename, O_RDONLY);
      if (fd < 0) {
	 Error("can't open file");
      }
      if (read(fd, &hdr, sizeof(hdr)) != sizeof(hdr)) {
	 Error("can't read file header");
      }

      if (print_filename) printf("\n%s:\n", filename);


      midmagic = *(long*)&hdr;
      printf("a.out ID: \t(0x) %04x %04x\n",
	     (midmagic & 0xffff0000) >> 16, midmagic & 0xffff);

      if (N_BADMAG(hdr) && !force) {
	 /* Don't bother doing any more unless -f flag: */
	 continue;
      }

      if (!swap) {
	 print_magic ((void*) &hdr);
	 printf("TEXT:   \t0x%x (addr) 0x%x (size) 0x%x (offset)\n",
		N_TXTADDR(hdr), hdr.a_text, N_TXTOFF(hdr));
	 printf("DATA:   \t0x%x (addr) 0x%x (size) 0x%x (offset)\n",
		N_DATADDR(hdr), hdr.a_data, N_DATOFF(hdr));
	 printf("BSS:    \t0x%x (addr) 0x%x (size)\n",
		N_BSSADDR(hdr), hdr.a_bss);
	 printf("a_entry: \t0x%x\n", hdr.a_entry);
	 printf("SYMBOLS: \t0x%x (size) 0x%x(offset)\n",
		hdr.a_syms, N_SYMOFF(hdr));
	 printf("STRINGS: \t??? (size) 0x%x(offset)\n",
		N_STROFF(hdr));
	 printf("TRELs:   \t0x%x (size) 0x%x(offset)\n",
		hdr.a_trsize, N_TRELOFF(hdr));
	 printf("DRELs:   \t0x%x (size) 0x%x(offset)\n",
		hdr.a_drsize, N_DRELOFF(hdr));
      } else {
	 /* If things are swapped, then we can't really use N_TXTADDR() and
	  * so forth.  Therefore just dump out the raw struct: */
	 printf("a_text: \t0x%x\n",   byteswap (hdr.a_text));
	 printf("a_data: \t0x%x\n",   byteswap (hdr.a_data));
	 printf("a_bss:  \t0x%x\n",   byteswap (hdr.a_bss));
	 printf("a_syms: \t0x%x\n",   byteswap (hdr.a_syms));
	 printf("a_entry: \t0x%x\n",  byteswap (hdr.a_entry));
	 printf("a_trsize: \t0x%x\n", byteswap (hdr.a_trsize));
	 printf("a_drsize: \t0x%x\n", byteswap (hdr.a_drsize));
      }


      if (!force && is_dynamic(&hdr)) {

	 void* fp;
	 void* base;
	 void* dp;

	 int fsize = hdr.a_text + hdr.a_data + hdr.a_syms; /* roughly */
	 fp = mmap(0, fsize, PROT_READ, MAP_SHARED, fd, 0);
	 if ((int)fp == -1) {
	    Error("Can't mmap file");
	 }
	 base = fp - N_TXTADDR(hdr); /* Add this to all "address"      */
				     /* values you find in the file to */
				     /* get the location at which it's */
				     /* currently mapped.              */


	 dp = fp + N_DATOFF(hdr); /* __DYNAMIC is always the first symbol */

	 print_dynamic (dp, base, fp);
      }
   }
   exit(0);
}







/************************** 
 *  NetBSD a.out routines *
 **************************/

#if defined(__NetBSD__) || defined(__FreeBSD__)

inline int
is_dynamic (struct exec* hdr)
{
   return N_GETFLAG(*hdr) & EX_DYNAMIC;
}


void
print_magic (struct exec* hdr)
{
   int dynamic = is_dynamic (hdr);

   printf("DYNAMIC: \t%s\n", dynamic ? "YES" : "NO");
   printf("PIC: \t\t%s\n", N_GETFLAG(*hdr) & EX_PIC ? "YES" : "NO");
   printf("MACHTYPE: \t");
   switch (N_GETMID(*hdr)) {
   case MID_ZERO:
      printf("Unknown\n");
      break;
#ifdef MID_M68K
   case MID_M68K:
      printf("MID_M68K\n");
      break;
#endif
   case MID_SPARC:
      printf("MID_SPARC\n");
      break;
   case MID_I386:
      printf("MID_I386\n");
      break;
   default:
   }
   printf("MAGIC: \t\t");
   switch (N_GETMAGIC(*hdr)) {
   case OMAGIC:
      printf("O (old)\n");
      break;
   case NMAGIC:
      printf("N (swapable)\n");
      break;
   case ZMAGIC:
      printf("Z (pageable)\n");
      break;
#ifdef QMAGIC
   case QMAGIC:
      printf("Q (compact)\n");
      break;
#endif
   default:
      printf("Not a "
#ifdef __NetBSD__
	     "NetBSD"
#elif __FreeBSD__
	     "FreeBSD"
#else
	     "recognized BSD"
#endif
	     " a.out executable\n");
      /* continue; */
   }
}


void
print_dynamic (void* p, void* base, void* fp)
{
   struct _dynamic* dp = p;

   printf("LD VERSION: \t%d\n", dp->d_version);
   if (dp->d_version == LD_VERSION_BSD) {
      struct section_dispatch_table* sdt;
      struct sod* lobj;

      sdt = base + (long)dp->d_un.d_sdt;
      printf("LIB PATH: \t%s\n", (char*) base + sdt->sdt_paths);
      printf("LIBRARIES:\n");

      lobj = base + sdt->sdt_sods;
      while ((void*)lobj > fp) {
	 printf("\t\t    lib %s (version %d.%d)\n",
		(char*)base + lobj->sod_name,
		lobj->sod_major,
		lobj->sod_minor);
	 lobj = base + lobj->sod_next;
      }
   } else {
      printf("Old LD Version not understood by this program...\n");
   }
}

#endif /* __NetBSD__ */




/************************* 
 *  SunOS a.out routines *
 *************************/

#ifdef sun

inline int
is_dynamic (struct exec* hdr)
{
   return hdr->a_dynamic;
}


void
print_magic (struct exec* hdr)
{
   int dynamic = hdr->a_dynamic;

   printf("DYNAMIC: \t%s\n", (dynamic ? "YES" : "NO"));
   printf("TOOL VERSION: \t%d\n", hdr->a_toolversion);
   printf("MACHTYPE: \t");
   switch (hdr->a_machtype) {
   case 0:
      printf("Pre-3.0\n");
      break;
   case M_68010:
      printf("M_68010\n");
      break;
   case M_68020:
      printf("M_68020\n");
      break;
   case M_SPARC:
      printf("M_SPARC\n");
      break;
   default:
   }
   printf("MAGIC: \t\t");
   switch (hdr->a_magic) {
   case OMAGIC:
      printf("O (old)\n");
      break;
   case NMAGIC:
      printf("N (swapable)\n");
      break;
   case ZMAGIC:
      printf("Z (pageable)\n");
      break;
   default:
      printf("Not a Sun a.out executable\n");
      /* continue; */
   }
}


void
print_dynamic (void* p, void* base, void* fp)
{
   struct link_dynamic* dp = p;

   printf("LD VERSION: \t%d\n", dp->ld_version);
   if (dp->ld_version >= 2) {
      struct link_dynamic_2* ldp = base + (int)dp->ld_un.ld_2;
      struct link_object* lobj;

      printf("LIB PATH: \t%s\n", (char*)fp + ldp->ld_rules);
      printf("LIBRARIES:\n");

      lobj = fp+ldp->ld_need;
      while ((void*)lobj > fp) {
	 printf("\t\t    lib %s (version %d.%d)\n",
		(char*)fp + lobj->lo_name,
		lobj->lo_major,
		lobj->lo_minor);
	 lobj = fp + lobj->lo_next;
      }
   } else {
      printf("Old LD Version not understood by this program...\n");
   }
}



#if 0
/* What is this for ?? */
unsigned int FindReloc(struct reloc_info_sparc* reloc,
		       unsigned int reloc_len,
		       unsigned int idx)
{
   struct reloc_info_sparc* end = (void*) reloc + reloc_len;
   while (reloc < end) {
      if (reloc->r_index == idx) {
	 return reloc->r_address;
      }
      reloc++;
   }
   return -1;			/* Not found */
}
#endif



#endif /* sun */
