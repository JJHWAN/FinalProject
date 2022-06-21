#include <linux/ioctl.h>

#define DEBUG

#define DEVICE_MAJOR 242
#define DEVICE_MINOR 0
#define DEVICE_NAME "/dev/driver"

// data from user by IOCTL_SET_OPTION
struct data_from_user{
    int score; // 점수
};

#define IOCTL_UPDATE_SCORE _IOW(DEVICE_MAJOR, 0, struct data_from_user)
#define IOCTL_WAIT_INTR _IO(DEVICE_MAJOR, 1)

