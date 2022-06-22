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

    struct data_from_user set_data;

    set_data.data = 1200;
    int fd, tmp;
    fd = open(DEVICE_NAME, O_WRONLY);

    if(fd < 0){
        printf("Device cannot be opened : %s\n", DEVICE_NAME);
        return -1;
    }

    if(ioctl(fd, IOCTL_UPDATE_SCORE, &set_data) < 0){
        printf("Iotcl Error : CMD number 0\n");
        return -1;
    }

   
    if((tmp = write(fd, " \n", 2)) < 0){
        printf("Write Error : CMD number %d\n", tmp);
        return -1;
    }
    printf("Write returned %d\n", tmp);

    set_data.data = 1;

    if(ioctl(fd, IOCTL_MSG, &set_data) < 0){
        printf("Iotcl Error : CMD number 0\n");
        return -1;
    }

    if(close(fd) < 0){
        printf("Close error!\n");
    }

    return 0;
}
