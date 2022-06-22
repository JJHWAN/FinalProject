CC=arm-none-linux-gnueabi-gcc -static -Wall -lm

default: app.o
	$(CC) -o app app.o 
	@echo ">>> BUILD COMPLETE (HW2_20171691)"
	@echo ">>> Type 'adb push ./app /data/local/tmp' to push to ARM board."
	@echo ">>> Or do 'make push' to build and push."
	
push: default
	@echo ">>> Pushing build file to ARM board..."
	adb push ./app /data/local/tmp
	@echo ">>> PUSH COMPLETE"

app.o : device.h app.c
	$(CC) -c -o app.o app.c

clean :
	-rm *.o
	-rm app