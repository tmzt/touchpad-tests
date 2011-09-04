#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>


/*
5806  open("/dev/ctp_uart", O_RDONLY|O_NONBLOCK) = 23                                          
5806  ioctl(23, 0x80086804, 0x7eaa1a10) = 0        
5806  ioctl(23, 0x40086805, 0x7eaa1a10) = 0

5806  open("/sys/devices/platform/cy8ctma395/vdd", O_WRONLY) = 24
5806  open("/sys/devices/platform/cy8ctma395/xres", O_WRONLY) = 25
5806  open("/sys/user_hw/pins/ctp/wake/level", O_WRONLY) = 26
5806  write(2, "HidInit:\n  ", 11)      = 11                                                   
5806  write(2, "Cy8ctma300Open Succeded\n", 24) = 24    
5806  write(2, "\n", 1)                 = 1                                                    
5806  write(24, "1", 1)                 = 1   
5806  write(26, "1", 1)                 = 1                                                 
5806  write(25, "1", 1)                 = 1
5806  write(25, "0", 1)                 = 1  

## Enabling touchpanel:

5806  ioctl(23, 0x4004680e, 0x9)        = 0                                                    
5806  write(26, "0", 1)                 = 1                                                    
5806  ioctl(22, 0x707, 0x7eaa1920)      = 1                                                    
5806  ioctl(22, 0x707, 0x7eaa1920)      = 1                                                    
5806  ioctl(22, 0x707, 0x7eaa1920)      = 1                                                    
5806  ioctl(22, 0x707, 0x7eaa1920)      = 1                                                    
5806  ioctl(22, 0x707, 0x7eaa1920)      = 1                                                    
5806  ioctl(22, 0x707, 0x7eaa1920)      = 1                                                    
5806  ioctl(22, 0x707, 0x7eaa1920)      = 1 

*/

int main() {
	int fd;
	fd = open("/dev/ctp_uart", O_RDONLY | O_NONBLOCK);
	char buf[1024];
	char *data = &buf;
	ioctl(fd, 0x80086804, (void *)data);
	write(0, data, 1024);
	ioctl(fd, 0x40086805, (void *)data);
	
	
	int vdd_fd = open("/sys/devices/platform/cyctma395/vdd", O_WRONLY);
	int xres_fd = open("/sys/devices/platform/cyctma395/xres", O_WRONLY);
	int level_fd = open("/sys/user_hw/pins/ctp/wake/level", O_WRONLY);
	write(vdd_fd, "1", 1);
	write(xres_fd, "1", 1);	
	write(xres_fd, "0", 1);
	
	printf("enabling touchpanel\n");
	ioctl(fd, 0x4004680e, 0x9);
	
	
}

