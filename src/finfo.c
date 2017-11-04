/* 11 Feb 93*/
/*
 * finfo.c
 *
 * Dump interesting information about executable files in a.out format.
 *
 * Copyright (c) 1993 Terrence R. Lambert. (terry@cs.weber.edu)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Terrence R. Lambert.
 * 4. The name Terrence R. Lambert may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY TERRENCE R. LAMBERT ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE TERRENCE R. LAMBERT BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Modification history
 * ---------	----------------------  -----------------------------------
 * 11 Feb 93	Terry Lambert		Original
 * ---------	----------------------  -----------------------------------
 */

#include <stdio.h>
#include <a.out.h>
#include <sys/file.h>
#ifdef USE_MMAP
#include <sys/types.h>
#include <sys/mman.h>
#endif



main( ac, av)
int	ac;
char	*av[];
{
	char		*fname;
	int		fd;
	struct exec	info_buf, *execp;	

	if( ac != 2) {
		fprintf( stderr, "usage: finfo <file>\n");
		exit( 1);
	}

	fname = av[ 1];

	if( ( fd = open( fname, O_RDONLY, 0)) == -1) {
		perror( "open");
		exit( 2);
	}

#ifdef USE_MMAP
	execp = (struct exec*)
	   mmap(0, sizeof(struct exec), PROT_READ, MAP_SHARED, fd, 0);
	if( (int)execp == -1) {
		perror( "mmap");
		close( fd);
		exit( 3);
	}
#else
	execp = &info_buf;
	if( read( fd, &info_buf, sizeof(struct exec)) == -1) {
		perror( "read");
		close( fd);
		exit( 3);
	}
#endif

	/*
	 * Magic number...
	 */

	if( N_BADMAG(*execp)) {
		fprintf( stderr, "Not an a.out format file\n");
		exit( 4);
	}

	/*
	 * Interesting data
	 */
	printf( "EXECUTABLE FILE:      %s\n", fname);
	printf( "TEXT ADDRESS:         %08x\n", N_TXTADDR(*execp));
	printf( "DATA ADDRESS:         %08x\n", N_DATADDR(*execp));
	printf( "TEXT OFFSET:          %08x\n", N_TXTOFF(*execp));
	printf( "DATA OFFSET:          %08x\n", N_DATOFF(*execp));
	printf( "SYMBOL TABLE  OFFSET: %08x\n", N_SYMOFF(*execp));
	printf( "STRING TABLE OFFSET:  %08x\n", N_STROFF(*execp));
	printf( "-a_text:              %08x\n", execp->a_text);
	printf( "-a_data:              %08x\n", execp->a_data);
	printf( "-a_bss:               %08x\n", execp->a_bss);
	printf( "-a_syms:              %08x\n", execp->a_syms);
	printf( "-a_entry:             %08x\n", execp->a_entry);
	printf( "-a_trsize:            %08x\n", execp->a_trsize);
	printf( "-a_drsize:            %08x\n", execp->a_drsize);

	close( fd);
}

/*
 * EOF -- This file has not been truncated.
 */
