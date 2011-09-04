#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

/*
5806  open("/dev/ctp_uart", O_RDONLY|O_NONBLOCK) = 23                                          
5806  ioctl(23, 0x80086804, 0x7eaa1a10) = 0        
5806  ioctl(23, 0x40086805, 0x7eaa1a10) = 0

5806  open("/sys/bus/platform/devices/cy8ctma395/vdd", O_WRONLY) = 24
5806  open("/sys/bus/platform/cy8ctma395/xres", O_WRONLY) = 25
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

int writebytes(int fd, char* bytes, int len) {
	write(fd, bytes, len);
}

int main() {
	int uart_fd = open("/dev/ctp_uart", O_RDONLY | O_NONBLOCK);
	char buf[1024];
	char *data = &buf;
	ioctl(uart_fd, 0x80086804, (void *)data);
	write(0, data, 1024);
	ioctl(uart_fd, 0x40086805, (void *)data);
	
	
	int vdd_fd = open("/sys/bus/platform/devices/cy8ctma395/vdd", O_WRONLY);
	int xres_fd = open("/sys/bus/platform/devices/cy8ctma395/xres", O_WRONLY);
	int wake_fd = open("/sys/user_hw/pins/ctp/wake/level", O_WRONLY);
	writebytes(vdd_fd, "1", 1);
	writebytes(xres_fd, "1", 1);	
	writebytes(xres_fd, "0", 1);
	
	printf("enabling touchpanel\n");
	ioctl(uart_fd, 0x4004680e, 0x9);

	
	memset(buf, '\0', 1024);
	snprintf(data, 2, "\b");
	
	struct i2c_msg msg;
	msg.addr = 0x67;
	msg.flags = 0x02;
	msg.buf = data;	
	struct i2c_msg* msgs = (struct i2c_msg *)&msg;
	struct i2c_rdwr_ioctl_data rdwr;
	rdwr.msgs = msgs;
	rdwr.nmsgs = 1;
	struct i2c_rdwr_ioctl_data * prdwr = (struct i2c_rdwr_ioctl_data *)&rdwr;
	
	writebytes(wake_fd, "0", 1);

	int i2c5_fd = open("/dev/i2c-5", O_RDWR);
	ioctl(i2c5_fd, 0x0707, prdwr);
	ioctl(i2c5_fd, 0x0707, prdwr);
	ioctl(i2c5_fd, 0x0707, prdwr);
	ioctl(i2c5_fd, 0x0707, prdwr);
	ioctl(i2c5_fd, 0x0707, prdwr);
	ioctl(i2c5_fd, 0x0707, prdwr);
	ioctl(i2c5_fd, 0x0707, prdwr);
	ioctl(i2c5_fd, 0x0707, prdwr);
	
	writebytes(wake_fd, "1", 1);
	printf("wake done\n");	
	
	while(1) {
		int nread = read(uart_fd, data, 1024);
		write(1, data, nread);
	}
}

