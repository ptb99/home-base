#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define VER     "0.1"
#define BUFFSZ  8192



unsigned char *cvs_pwd(unsigned char *pwd);



int main(int argc, char *argv[]) {
    FILE            *fd;
    unsigned char   buff[BUFFSZ + 1],
                    *ptr,
                    *user,
                    *pass;


    setbuf(stdout, NULL);

    fputs("\n"
        ".cvspass decoder "VER"\n"
        "by Luigi Auriemma\n"
        "e-mail: aluigi@autistici.org\n"
        "web:    aluigi.org\n"
        "\n", stdout);

    if(argc < 2) {
        printf("\nUsage: %s <password_in_.cvspass_OR_.cvspass>\n"
            "\n"
            "Note: this tool can accept both the encoded password in .cvspass or directly the .cvspass file\n"
            "\n", argv[0]);
        exit(1);
    }

    fd = fopen(argv[1], "rt");
    if(!fd) {
        printf("Password:   %s\n", cvs_pwd(argv[1]));
    } else {
        fputs("Stored passwords:\n", stdout);
        while(fgets(buff, BUFFSZ, fd)) {
            ptr = strchr(buff, ':');
            if(!ptr) continue;
            user = strchr(ptr + 1, ':');
            if(!user) continue;
            user++;
            ptr = strchr(user, ':');
            if(!ptr) continue;
            *ptr++ = 0;
            ptr = strchr(ptr, ' ');
            if(!ptr) continue;
            pass = cvs_pwd(ptr + 1);
            if(*pass > '\r') {
                printf(
                    "User        %s\n"
                    "Password:   %s",
                    user, pass);
            }
        }
        fclose(fd);
    }

    return(0);
}



unsigned char *cvs_pwd(unsigned char *pwd) {
    unsigned char   *ret = pwd + 1;

    static unsigned char    shifts[] = {
        0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
       16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
      114,120, 53, 79, 96,109, 72,108, 70, 64, 76, 67,116, 74, 68, 87,
      111, 52, 75,119, 49, 34, 82, 81, 95, 65,112, 86,118,110,122,105,
       41, 57, 83, 43, 46,102, 40, 89, 38,103, 45, 50, 42,123, 91, 35,
      125, 55, 54, 66,124,126, 59, 47, 92, 71,115, 78, 88,107,106, 56,
       36,121,117,104,101,100, 69, 73, 99, 63, 94, 93, 39, 37, 61, 48,
       58,113, 32, 90, 44, 98, 60, 51, 33, 97, 62, 77, 84, 80, 85,223,
      225,216,187,166,229,189,222,188,141,249,148,200,184,136,248,190,
      199,170,181,204,138,232,218,183,255,234,220,247,213,203,226,193,
      174,172,228,252,217,201,131,230,197,211,145,238,161,179,160,212,
      207,221,254,173,202,146,224,151,140,196,205,130,135,133,143,246,
      192,159,244,239,185,168,215,144,139,165,180,157,147,186,214,176,
      227,231,219,169,175,156,206,198,129,164,150,210,154,177,134,127,
      182,128,158,208,162,132,167,209,149,241,153,251,237,236,171,195,
      243,233,253,240,194,250,191,155,142,137,245,235,163,242,178,152
    };

    if(*pwd != 'A') return("unknown encoding method");
    pwd++;
    while(*pwd) {
        *pwd = shifts[*pwd];
        pwd++;
    }
    return(ret);
}

