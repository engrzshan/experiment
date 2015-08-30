#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define BAUDRATE B115200
#define USBDEVICE "/dev/ttyUSB0"

/* everything is main, subroutines are overrated */
int main(int argc, char *argv[])
{
  int fd, rc, res;
  struct termios oldtio,newtio;
  unsigned char buf[IPBUFSIZ];
  unsigned char cmd_en_gal[] = 
  {0xB5,0x62,0x06,0x3E,0x34,0x00,0x00,0x00,0x20,0x06,0x00,0x08,0x10,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x00,0x01,0x01,0x02,0x08,0x0A,0x00,0x01,0x00,0x01,0x01,0x03,0x08,0x10,0x00,0x00,0x00,0x01,0x01,0x05,0x00,0x03,0x00,0x00,0x00,0x01,0x01,0x06,0x08,0x0E,0x00,0x00,0x00,0x01,0x01,0x1B,0x31};
  unsigned char cmd_en_zda[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x08,0x00,0x01,0x00,0x00,0x00,0x00,0x08,0x60};
  unsigned char cmd_change_talkerid[] = {0xB5,0x62,0x06,0x17,0x14,0x00,0x00,0x41,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x78};
  unsigned int sigma, delta;
 
  fd = open(USBDEVICE, O_RDWR | O_NOCTTY );
  if (fd <0) {
    fprintf(stderr,"Error: Can't open device %s\n",USBDEVICE);
    perror(USBDEVICE); /* crash and burn */
    exit(-1);
  }
 
  tcgetattr(fd,&oldtio);
  bzero(&newtio, sizeof(newtio));
  newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR | ICRNL;
  newtio.c_oflag = 0;
  newtio.c_lflag = ICANON;
 
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);
 
  res = write(fd, cmd_en_gal, sizeof(cmd_en_gal));
  sleep(1);
  if(res<0){
    printf("Failed to write to serial port\n");
  } 
  else {
    printf("Bytes written=%d\n", res);
  }
  
  res = write(fd, cmd_en_zda, sizeof(cmd_en_zda));
  sleep(1);
  if(res<0){
    printf("Failed to write to serial port\n");
  } 
  else {
    printf("Bytes written=%d\n", res);
  }

  
  res = write(fd, cmd_change_talkerid, sizeof(cmd_change_talkerid));
  sleep(1);
    if(res<0){
    printf("Failed to write to serial port\n");
  } 
  else {
    printf("Bytes written=%d\n", res);
  }
 
  /* pointless code as should never get here */
  tcsetattr(fd,TCSANOW,&oldtio);
  close(fd);
  exit(0);
}