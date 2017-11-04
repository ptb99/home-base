#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>


extern char *crypt();
extern char *getenv();

#define PASSLENGTH 20
static char  userpass[PASSLENGTH];
static char  rootpass[PASSLENGTH];
static char *user = NULL;




static int
getPassword()
{

   char        buffer[PASSLENGTH];
   int         y, left;
   int         done = 0;
   struct passwd *pw;

   pw = (struct passwd *)getpwnam("root");
   (void) strcpy(rootpass, pw->pw_passwd);

#ifdef SOLARIS_SHADOW
   pw = (struct passwd *)getpwnam(cuserid(NULL));
#else
   pw = (struct passwd *)getpwuid(getuid());
#endif
   (void) strcpy(userpass, pw->pw_passwd);
   user = pw->pw_name;



   printf("Username: %s\n", user);
   while (!done) {
      char* p;
      char* res;

      p = getpass("Password:");

      /*
       * we don't allow for root to have no password, but we handle the case
       * where the user has no password correctly; they have to hit return
       * only
       */
      res = crypt(p, userpass);
      done = strcmp(res, userpass) == 0;
      res = crypt(p, rootpass);
      done = done || strcmp(crypt(p, rootpass), rootpass) == 0;

      if (!done && !*p) {
	 /* just hit return, and it wasn't his password */
	 break;
      }
      if (!*userpass && *p) {
	 /*
	  * the user has no password, but something was typed anyway.
	  * sounds fishy: don't let him in...
	  */
	 done = 0;
      }



      if (done) {
	 printf("Valid...\n");
	 return 0;
      } else {
	 (void) sleep(1);
	 printf("Invalid passwd\n");
      }
   }
   return 1;
}


main()
{
   int result = 1;
   while (result) {
      result = getPassword();
   }
}
