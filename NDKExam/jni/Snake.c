#include <jni.h>
#include "android/log.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include "device.h"
/*
 * Class:     org_example_ndk_Snake
 * Method:    openDevice
 * Signature: ()I
 */

#define LOG_TAG "MyTag"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

int fd;

JNIEXPORT jint JNICALL Java_org_example_ndk_Snake_openDevice
  (JNIEnv *env, jclass cls){

	fd = open(DEVICE_NAME, O_RDWR, 777);
	LOGV("While open result %d\n", fd);
	return (jint)fd;
}

/*
 * Class:     org_example_ndk_Snake
 * Method:    closeDevice
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_example_ndk_Snake_closeDevice
  (JNIEnv *env, jclass cls){
	close(fd);
}

/*
 * Class:     org_example_ndk_Snake
 * Method:    getDirFromBoard
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_example_ndk_Snake_getDirFromBoard
  (JNIEnv *env, jobject obj){
	int dir = write(fd, " \n", 2);
	LOGV("DIR result %d\n", dir);
	return dir;
}

/*
 * Class:     org_example_ndk_Snake
 * Method:    updateScore
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_example_ndk_Snake_updateScore
  (JNIEnv *env, jclass cls, jlong score){

	struct data_from_user data;
	long tmp = (long)score;
	data.data = (int)tmp;
	ioctl(fd, IOCTL_UPDATE_SCORE, &data);
	LOGV("Updated Score to %d\n", data.data);
}

/*
 * Class:     org_example_ndk_Snake
 * Method:    msgBoard
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_example_ndk_Snake_msgBoard
  (JNIEnv *env, jclass cls, jint jflag){
	int flag = (int)jflag;
	struct data_from_user data;
	data.data = flag;
	ioctl(fd, IOCTL_MSG, &data);
	LOGV("MSG Board MSG num : %d\n", flag);
}
