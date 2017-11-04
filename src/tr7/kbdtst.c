#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <machine/console.h>



main()
{
   int ktype;
   int vmode;
   int kbmode;
   long syscons_version;
   int nchar;
   char buf[256];
   int i;
   int fd;
   struct termios oldt, nterm;

   fd = open("/dev/tty", O_RDWR);
   if (fd < 0) {
      perror("open tty");
      exit(EXIT_FAILURE);
   }

   if (ioctl(fd, CONS_GETVERS, &syscons_version) < 0) {
      syscons_version = 0;
   }
   printf("Syscons version: %d.%d\n", syscons_version >> 8,
	  syscons_version & 0xff);


#ifdef NOTYET
   if (ioctl(fd, KDGKBTYPE, &ktype) == -1)
      perror("KDGKBTYPE");
   printf("Keyboard type: ");
   switch (ktype) {
   case KB_84:
      printf("KB_84 (84-key old style keyboard)\n");
      break;
   case KB_101:
      printf("KB_101 (101-key or 102-key standard keyboard)\n");
      break;
   case KB_OTHER:
      printf("KB_OTHER (unknown keyboard)\n");
      break;
   default:
      printf("bad kb code (%d)\n", ktype);
   }
#endif

   if (ioctl(fd, KDGETMODE, &vmode) == -1)
      perror("KDGETMODE");
   printf("Current video mode: ");
   switch (vmode) {
   case KD_GRAPHICS:
      printf("KD_GRAPHICS\n");
      break;
   case KD_TEXT1:
      printf("KD_TEXT1\n");
      break;
   case KD_TEXT:
      printf("KD_TEXT\n");
      break;
   default:
      printf("unknown (0x%x)\n", vmode);
   }   


   if (ioctl(fd, KDGKBMODE, &kbmode) == -1)
      perror("KDGKBMODE");
   printf("Current kbd translation mode: ");
   switch (kbmode) {
   case K_RAW:
      printf("K_RAW\n");
      break;
   case K_XLATE:
      printf("K_XLATE\n");
      break;
   default:
      printf("unknown (0x%x)\n", kbmode);
   }   


   /* Save away the old termios info: */
   if (tcgetattr(fd, &oldt) < 0)
      perror("tcgetattr");

   /* Set the term into raw mode (no cr required) */
   nterm = oldt;
#if 0
   nterm.c_iflag = IGNPAR | IGNBRK;
   nterm.c_oflag = 0;
   nterm.c_cflag = CREAD | CS8;
   nterm.c_lflag = 0;
#endif
   cfsetispeed(&nterm, 9600);
   cfsetospeed(&nterm, 9600);
   cfmakeraw(&nterm);
   nterm.c_cc[VTIME] = 10;
   nterm.c_cc[VMIN] = 16;
   if (tcsetattr(fd, TCSANOW, &nterm) < 0)
      perror("tcsetattr raw");

   if (ioctl(fd, KDSKBMODE, K_RAW) == -1) /* switch to RAW scancode mode */
      perror("KDSKBMODE");



   nchar = read(fd, buf, 256);
   if (nchar < 1)
      perror("read char");


   /* Restore tty state: */
   kbmode = K_XLATE;
   if (ioctl(fd, KDSKBMODE, kbmode) == -1) /* switch back to normal mode */
      perror("KDSKBMODE");
   if (tcsetattr(fd, TCSAFLUSH, &oldt) < 0)
      perror("tcsetattr restore");


   /* print out result: */
   for (i=0; i<nchar; i++) {
      printf("%4d 0x%2x\n", buf[i], buf[i] & 0xff);
   }
}
