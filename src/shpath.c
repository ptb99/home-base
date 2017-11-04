#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
     extern char *optarg;
     extern int optind;

     int c;
     int first=0;
     int rc;
          
     char pathtype[80]="PATH";
     char name[80]="";
     char *thepath, *dir, *file;
     char thefile[512];
     
     struct stat statinfo;
     
     while ((c = getopt( argc, argv, "fp:")) != -1)
     {
	  
	  switch (c)
	  {
	  case 'f':
	       first = 1;
	       break;
	  case 'p':
	       sscanf(optarg,"%s",pathtype);
	       break;
	  }
     }
     
     if (optind < argc)
     {
	  sscanf(argv[optind],"%s",name);
	  optind++;
     }

     thepath = getenv( pathtype );
     if (strlen(thepath) == 0) exit(1);
     
     for (dir = strtok(thepath,":"); dir != NULL; dir = strtok(NULL,":"))
     {
	  if (strlen(name) == 0)
	       file = dir;
	  else
	  {
	       file = thefile;
	       strcpy(thefile,dir);
	       strcat(thefile,"/");
	       strcat(thefile,name);
	  }
	  
	  if (stat(file,&statinfo) == 0)
	  {
	       printf("%s\n", file);
	       if (first) break;
	  }
     }

     exit(0);
}

