all:
	gcc -c WMS_SHM.c
	gcc -c WMS_API.c
	gcc -c WMS_Test.c
	gcc -c WMS_Blob.c
	gcc -c WMS_Daemon.c
	gcc -c ENiX_LIST.c
	gcc -c ENiX_WMS.c
	gcc -c ENiX_CALC.c
	gcc -c ENiX_DISK.c
	gcc -c ENiX_TIME.c
	gcc -c ENiX_STRING.c
	gcc -c WitchCraft-Net.c
	gcc WMS_SHM.o WMS_Blob.o ENiX_LIST.o ENiX_WMS.o ENiX_CALC.o ENiX_STRING.o ENiX_DISK.o ENiX_TIME.o WMS_Test.o -o WMS-Test.bin -lm
	gcc WitchCraft-Net.o WMS_SHM.o WMS_API.o WMS_Blob.o ENiX_LIST.o ENiX_WMS.o ENiX_CALC.o ENiX_STRING.o ENiX_DISK.o ENiX_TIME.o WMS_Daemon.o -o WMS-Daemon.bin -lm

clean:
	rm -f *~
	rm -f *.o
	rm -f WMS
