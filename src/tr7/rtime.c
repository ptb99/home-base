
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <stdio.h>
#include <time.h>


void Usage(char *prog)
{
   fprintf(stderr, "%s: [-t timeout] hostname(s) ...\n", prog);
   exit(1);
}


main(int argc, char *argv[])
{
   int c;
   extern int optind;
   extern char* optarg;
   int errflag = 0;

   int i;
   struct timeval timeout = {10, 0};
   

   while ((c = getopt(argc, argv, "t:")) != EOF) {
      switch (c) {
      case 't':
	 timeout.tv_sec = atoi(optarg);
         if (timeout.tv_sec == 0) errflag = 1;
	 break;
      default:
         errflag = 1 ;
         break;
      }
   }

   if (errflag || optind == argc) {
      Usage(argv[0]);
   }
      

   for ( i = optind; i < argc; i++ ) {
      
      char* host = argv[i];
      time_t thetime;
      struct hostent *hp;
      struct sockaddr_in sock;
      struct timeval result;
      
      if ((hp = gethostbyname(host)) == NULL) {
	 fprintf(stderr, "%s: can't get addr for %s\n", argv[0], host);
	 continue;
      }
      
      memcpy( &sock.sin_addr, hp->h_addr,  hp->h_length);
      sock.sin_family = AF_INET;
      sock.sin_port = 0;


      if (rtime(&sock, &result, &timeout) != 0) {
	 /* An error occurred while calling the server.
	  * Print error message and die.
	  */
	 perror(argv[0]);
	 fprintf(stderr, "%s: couldn't reach %s\n", argv[0], host);
      } else {
      
	 thetime = result.tv_sec;
	 printf("%12s: %s", host, ctime(&thetime));
      }
   }
}



#ifdef NEED_RTIME
int
rtime (struct sockaddr_in *sock,
       struct timeval *result,
       struct timeval *timeout)
{
    struct servent* service;
    int fd;
    long time;

    service = getservbyname("time", "tcp");
    if (! service) {
	perror("rtime");
	return 1;
    }
    sock->sin_port = service->s_port;

    fd = socket (AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
	perror("rtime");
	return 1;
    }
    if (connect (fd, (struct sockaddr*) sock, sizeof(*sock)) != 0) {
	perror("rtime");
	return 1;
    }
    read (fd, &time, sizeof(time));
    /* The magic constant 2208988800 seconds is the time between Jan. 1, 1900,
       and Jan. 1, 1970. */
    result->tv_sec = ntohl(time) - 2208988800UL;
    result->tv_usec = 0;

    return 0;
}
#endif
