#include <stdio.h>
#include <locale.h>
#include <gdk/gdk.h>

int
main()
{
    char* rc1;
    char* rc2;
    gchar* grc;

    rc1 = setlocale (LC_ALL, NULL);
    rc2 = setlocale(LC_CTYPE, NULL);
    printf ("starting locale: ALL= %s, CTYPE= %s\n",
	    rc1 ? rc1: "NULL", rc2 ? rc2: "NULL");

    rc1 = setlocale(LC_CTYPE, "");
    printf ("setlocale() returns %s\n",
	    rc1 ? rc1: "NULL");

    rc1 = setlocale (LC_ALL, NULL);
    rc2 = setlocale(LC_CTYPE, NULL);
    printf ("current locale: ALL= %s, CTYPE= %s\n",
	    rc1 ? rc1: "NULL", rc2 ? rc2: "NULL");


    grc = gdk_set_locale();
    printf ("gdk_set_locale() returns %s\n",
	    grc ? grc: "NULL");
    
}
