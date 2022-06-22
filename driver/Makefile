obj-m = snake_driver.o 
CC := arm-none-linux-gnueabi-gcc -static -Wall -lm

KDIR    :=/work/achroimx_kernel
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

push: default
	@echo ">>> Pushing build file to ARM board..."
	adb push ./dino_driver.ko /data/local/tmp
	@echo ">>> PUSH COMPLETE"

clean:
	rm -rf *.o
	rm -rf *.ko
	rm -rf *.mod.c
	rm -rf *.order
	rm -rf *.symvers
