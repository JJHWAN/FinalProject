/*
 * device.h
 *
 *  Created on: Jun 22, 2022
 *      Author: root
 */
#include <linux/ioctl.h>

#ifndef DEVICE_H_
#define DEVICE_H_

#define DEVICE_NAME "dev/driver"
#define DEVICE_MAJOR 242
#define DEVICE_MINOR 0

struct data_from_user{
	int data;
};

#define IOCTL_UPDATE_SCORE _IOW(DEVICE_MAJOR, 0, struct data_from_user)
#define IOCTL_MSG _IOW(DEVICE_MAJOR, 1, struct data_from_user)

#endif /* DEVICE_H_ */
