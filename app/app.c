// Test Application for HW2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "device.h"


int main(int argc, char **argv){

    if(argc != 4){
       	printf("Invalid Value Arguments!\n");
		return -1;
    }

    int timer_interval = atoi(argv[1]);
    int timer_cnt = atoi(argv[2]);
    int init_num = atoi(argv[3]);

    struct data_from_user set_data;

    set_data.score = 0;
    int fd;
    fd = open(DEVICE_NAME, O_WRONLY);

    if(fd < 0){
        printf("Device cannot be opened : %s\n", DEVICE_NAME);
        return -1;
    }

    if(ioctl(fd, IOCTL_UPDATE_SCORE, &set_data) < 0){
        printf("Iotcl Error : CMD number 0\n");
        return -1;
    }

    if(ioctl(fd, IOCTL_WAIT_INTR) < 0){
        printf("Iotcl Error : CMD number 0\n");
        return -1;
    }

    if(close(fd) < 0){
        printf("Close error!\n");
    }

    return 0;
}
