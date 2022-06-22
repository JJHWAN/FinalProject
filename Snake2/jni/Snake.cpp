#include <jni.h>
#include "android/log.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include "device.h"
/*
 * Class:     com_example_snake_Snake
 * Method:    openDevice
 * Signature: ()I
 */

int fd;

JNIEXPORT jint JNICALL Java_com_example_snake_Snake_openDevice
  (JNIEnv *env, jclass cls){
	fd = open(DEVICE_NAME, O_RDWR);
	return (jint)fd;
}

/*
 * Class:     com_example_snake_Snake
 * Method:    closeDevice
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_snake_Snake_closeDevice
  (JNIEnv *env, jclass cls){
	close(fd);
}

/*
 * Class:     com_example_snake_Snake
 * Method:    getDirFromBoard
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_snake_Snake_getDirFromBoard
  (JNIEnv *env, jobject obj){
	int dir = ioctl(fd, IOCTL_WAIT_INTR);
	return dir;
}

/*
 * Class:     com_example_snake_Snake
 * Method:    updateScore
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_example_snake_Snake_updateScore
  (JNIEnv *env, jclass cls, jlong score){

	struct data_from_user data;
	long tmp = (long)score;
	data.data = (int)tmp;
	ioctl(fd, IOCTL_UPDATE_SCORE, &data);
}

/*
 * Class:     com_example_snake_Snake
 * Method:    msgBoard
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_example_snake_Snake_msgBoard
  (JNIEnv *env, jclass cls, jint jflag){
	int flag = jflag;
	struct data_from_user data;
	data.data = flag;
	ioctl(fd, IOCTL_MSG, &data);
}
