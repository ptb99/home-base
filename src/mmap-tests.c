<html>
<head><title>FreeBSD Mail Archives</title>
<meta name="robots" content="nofollow">
</head>
<body text="#000000" bgcolor="#ffffff">
<IMG SRC="../gifs/bar.gif" ALT="Navigation Bar" WIDTH="565" HEIGHT="33" BORDER=0 usemap="#bar">
<map name="bar">
<area shape="rect" coords="1,1,111,31" href="../index.html" ALT="Top">
<area shape="rect" coords="112,11,196,31" href="../ports/index.html" ALT="Applications">
<area shape="rect" coords="196,12,257,33" href="../support.html" ALT="Support">
<area shape="rect" coords="256,12,365,33" href="../docs.html" ALT="Documentation"> 
<area shape="rect" coords="366,13,424,32" href="../commercial/commercial.html" ALT="Vendors">
<area shape="rect" coords="425,16,475,32" href="../search/search.html" ALT="Search">
<area shape="rect" coords="477,16,516,33" href="../search/index-site.html" ALT="Index">
<area shape="rect" coords="516,15,562,33" href="../index.html" ALT="Top">
<area shape="rect" href="../index.html" coords="0,0,564,32" ALT="Top">
</map><pre>
<strong>Date: </strong>     Sun, 11 Aug 1996 07:24:22 +1000 (EST)
<strong>From: </strong>     Julian Assange &lt;proff@suburbia.net&gt;
<strong>To: </strong>       freebsd-hackers@freebsd.org
<strong>Subject: </strong>  mmap
<strong>Message-ID: </strong> &lt;<a href="mid.cgi?db=irt&id=199608102124.HAA15697@suburbia.net">199608102124.HAA15697@suburbia.net</a>&gt;
</pre>
<HR NOSHADE>
<a href="mid.cgi?db=irt&id=199608102124.HAA15697@suburbia.net">Next in thread</a>
| <a href="/cgi/getmsg.cgi?fetch=608893+616934+/usr/local/www/db/text/1996/freebsd-hackers/19960804.freebsd-hackers+raw">Raw E-Mail</a>
| <a href="/mail/archive/1996/freebsd-hackers/19960804.freebsd-hackers.html">Index</a>
| <a href="/cgi/getmsg.cgi?fetch=608893+616934+/usr/local/www/db/text/1996/freebsd-hackers/19960804.freebsd-hackers+archive">Archive</a>
| <a href="../search/searchhints.html">Help</a>
<HR NOSHADE>
<p><pre>

Recently I wrote some mmap() testing routines for GNU autoconf, you
may be pleased to know that FreeBSD mmap seems faultless.

I'd be interested in seeing results from other platforms....


/* $Id: mmap_tests.c,v 1.1 1996/08/10 20:12:09 proff Exp $
 * 
 * various mmap() immplimentations suck, we attempt to find out just how
 * hard.
 *
 *        - Julian Assange (proff@suburbia.net)
 *
 * Test results: (please send additions to proff@suburbia.net)
 *
 *   Linux suburbia 2.0.0 #29- Thu Jul 11 18:03:20 EST 1996 i586 
 *
 *	HAVE_MMAP_FILE_PRIVATE_READ
 *	HAVE_MMAP_FILE_PRIVATE_CHILD_INHERIT
 *	HAVE_MMAP_FILE_PRIVATE_WRITE
 *	HAVE_MMAP_FILE_SHARED_READ
 *	HAVE_MMAP_FILE_SHARED_CHILD_INHERIT
 *	HAVE_MMAP_FILE_SHARED_CHILD_READ_PARENT_WRITE
 *	HAVE_MMAP_FILE_SHARED_PARENT_READ_CHILD_WRITE
 *	HAVE_MMAP_FILE_SHARED_WRITE
 *	HAVE_MMAP_DEV_ZERO_PRIVATE
 *	HAVE_MMAP_DEV_ZERO_PRIVATE_CHILD_INHERIT
 *	HAVE_MMAP_ANON_PRIVATE
 *	HAVE_MMAP_ANON_PRIVATE_CHILD_INHERIT
 *
 *   FreeBSD profane 2.2-CURRENT #0 Sat Jul 27 19:16:00 EST 1996
 *
 *	HAVE_MMAP_FILE_PRIVATE_READ
 *	HAVE_MMAP_FILE_PRIVATE_CHILD_INHERIT
 *	HAVE_MMAP_FILE_PRIVATE_WRITE
 *	HAVE_MMAP_FILE_SHARED_READ
 *	HAVE_MMAP_FILE_SHARED_CHILD_INHERIT
 *	HAVE_MMAP_FILE_SHARED_CHILD_READ_PARENT_WRITE
 *	HAVE_MMAP_FILE_SHARED_PARENT_READ_CHILD_WRITE
 *	HAVE_MMAP_FILE_SHARED_WRITE
 *	HAVE_MMAP_DEV_ZERO_PRIVATE
 *	HAVE_MMAP_DEV_ZERO_PRIVATE_CHILD_INHERIT
 *	HAVE_MMAP_DEV_ZERO_SHARED
 *	HAVE_MMAP_DEV_ZERO_SHARED_CHILD_INHERIT
 *	HAVE_MMAP_DEV_ZERO_SHARED_CHILD_READ_PARENT_WRITE
 *	HAVE_MMAP_DEV_ZERO_SHARED_PARENT_READ_CHILD_WRITE
 *	HAVE_MMAP_ANON_PRIVATE
 *	HAVE_MMAP_ANON_PRIVATE_CHILD_INHERIT
 *	HAVE_MMAP_ANON_SHARED
 *	HAVE_MMAP_ANON_SHARED_CHILD_INHERIT
 *	HAVE_MMAP_ANON_SHARED_CHILD_READ_PARENT_WRITE
 *	HAVE_MMAP_ANON_SHARED_PARENT_READ_CHILD_WRITE
 */

#include &lt;stdio.h&gt;
#include &lt;sys/types.h&gt;
#include &lt;fcntl.h&gt;
#include &lt;sys/mman.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;unistd.h&gt;
#include &lt;signal.h&gt;

#define MM_SIZE (100*1024)
#define TEST_FILE "mmap.test"

int caught_sigint;

void sigint (int sig)
{
	caught_sigint++;
	signal (SIGINT, sigint);
}

/* 
 * TODO: test MAP_INHERIT, MAP_FIXED  (can't see that latter being much of an issue)
 */

void
test_child(char *p, char *msg)
{
	char *im = "inherit_magic";
	char *pm = "parent_magic";
	char *cm = "child_magic";
	pid_t pid;
	strcpy (p, im);
	caught_sigint = 0;
	signal (SIGINT, sigint);
	fflush (stdout);
	pid = fork();
	if (pid&lt;0)
		return;
	if (pid==0)
	{
		if (strcmp(p, im)==0)
			printf("%s_CHILD_INHERIT\n", msg);
		kill (getppid(), SIGINT);
		while (!caught_sigint)
			pause ();
		caught_sigint = 0;
		if (strcmp(p, pm)==0)
			printf("%s_CHILD_READ_PARENT_WRITE\n", msg);
		strcpy (p, cm);
		fflush (stdout);
		kill (getppid(), SIGINT);
		exit(0);
	}
	/* parent */
	while (!caught_sigint)
		pause ();
	caught_sigint = 0;
	strcpy (p, pm);
	kill (pid, SIGINT);
	while (!caught_sigint)
		pause ();
	if (strcmp(p, cm)==0)
		printf("%s_PARENT_READ_CHILD_WRITE\n", msg);
	signal (SIGINT, SIG_DFL);
}

int
main()
{
	int fd;
	char *m = malloc(MM_SIZE);
	char buf[1024];
	signal (SIGSEGV, SIG_IGN); /* the strcmp's should still fail */
#if defined(MAP_PRIVATE)
	fd = open(TEST_FILE, O_RDWR|O_CREAT|O_TRUNC, 0666);
	strcpy (m, "mmap magic");
	write (fd, m, 11);
	lseek (fd, 0, SEEK_SET);
	if (fd&gt;=0)
	{
		char *p=(char *)mmap(0, MM_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
		if (p!=(char *)-1)
		{
			if (strcmp(p, m)==0)
			 	puts("HAVE_MMAP_FILE_PRIVATE_READ");
			test_child(p, "HAVE_MMAP_FILE_PRIVATE");
			strcpy (p, "mmap magic2");
			munmap (p, MM_SIZE);
			read (fd, buf, 12);
			if (strcmp(buf, "mmap magic2")!=0) /* we shouldn't be writing to the file in private mode */
				puts("HAVE_MMAP_FILE_PRIVATE_WRITE");
		}
		close (fd);
	}
#endif
#if defined(MAP_SHARED) /* we should fork off another process to test the sharing */
	fd = open(TEST_FILE, O_RDWR|O_CREAT|O_TRUNC, 0666);
	strcpy (m, "mmap magic");
	write (fd, m, 11);
	lseek (fd, 0, SEEK_SET);
	if (fd&gt;=0)
	{
		char *p=(char *)mmap(0, MM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (p!=(char *)-1)
		{
			if (strcmp(p, m)==0)
			 	puts("HAVE_MMAP_FILE_SHARED_READ");
			test_child(p, "HAVE_MMAP_FILE_SHARED");
			strcpy (p, "mmap magic2");
			munmap (p, MM_SIZE);
			read (fd, buf, 12);
			if (strcmp(buf, "mmap magic2")==0)
				puts("HAVE_MMAP_FILE_SHARED_WRITE");
		}
		close (fd);
	}
#endif
#if defined(MAP_PRIVATE)
	fd = open("/dev/zero", O_RDWR, 0666);
	if (fd&gt;=0)
	{
		char *p=(char *)mmap(0, MM_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
		if (p!=(char *)-1)
		{
			strcpy (p, "mmap magic_dev_zero");
			if (strcmp (p, "mmap magic_dev_zero")==0)
			 	puts("HAVE_MMAP_DEV_ZERO_PRIVATE");
			test_child(p, "HAVE_MMAP_DEV_ZERO_PRIVATE");
			munmap (p, MM_SIZE);
		}
		close (fd);
	}
#endif
#if defined(MAP_PRIVATE)
	fd = open("/dev/zero", O_RDWR, 0666);
	if (fd&gt;=0)
	{
		char *p=(char *)mmap(0, MM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (p!=(char *)-1)
		{
			strcpy (p, "mmap magic_dev_zero_shared");
			if (strcmp (p, "mmap magic_dev_zero_shared")==0)
			 	puts("HAVE_MMAP_DEV_ZERO_SHARED");
			test_child(p, "HAVE_MMAP_DEV_ZERO_SHARED");
			munmap (p, MM_SIZE);
		}
		close (fd);
	}
#endif
#if defined(MAP_ANONYMOUS) &amp;&amp; !defined(MAP_ANON)
#define MAP_ANON MAP_ANONYMOUS
#endif
#ifdef MAP_ANON
	{
		char *p=(char *)mmap(0, MM_SIZE, PROT_READ | PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
		if (p!=(char *)-1)
		{
			strcpy (p, "mmap magic_anon");
			if (strcmp(p, "mmap magic_anon")==0)
			 	puts("HAVE_MMAP_ANON_PRIVATE");
			test_child(p, "HAVE_MMAP_ANON_PRIVATE");
			munmap (p, MM_SIZE);
		}
	}
#endif
#if defined(MAP_ANON) &amp;&amp; defined(MAP_SHARED) /* we should fork off another process to test the sharing */
	{
		char *p=(char *)mmap(0, MM_SIZE, PROT_READ | PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);
		if (p!=(char *)-1)
		{
			strcpy (p, "mmap magic_shared_anon");
			if (strcmp(p, "mmap magic_shared_anon")==0)
			 	puts("HAVE_MMAP_ANON_SHARED");
			test_child(p, "HAVE_MMAP_ANON_SHARED");
			munmap (p, MM_SIZE);
		}
	}
#endif
	unlink(TEST_FILE);
	exit (0);
}
-- 
"Of all tyrannies a tyranny sincerely  exercised for the good of its victims  
 may be the most  oppressive.  It may be better to live under  robber barons  
 than  under  omnipotent  moral busybodies,  The robber baron's  cruelty may  
 sometimes sleep,  his cupidity may at some point be satiated; but those who  
 torment us for own good  will torment us  without end,  for they do so with 
 the approval of their own conscience."    -   C.S. Lewis, _God in the Dock_ 
+---------------------+--------------------+----------------------------------+
|Julian Assange RSO   | PO Box 2031 BARKER | Secret Analytic Guy Union        |
|proff@suburbia.net   | VIC 3122 AUSTRALIA | finger for PGP key hash ID =     |
|proff@gnu.ai.mit.edu | FAX +61-3-98199066 | 0619737CCC143F6DEA73E27378933690 |
+---------------------+--------------------+----------------------------------+


</pre>
<hr><address><a href="../mailto.html">www@freebsd.org</a><br></address>
</BODY></HTML>
