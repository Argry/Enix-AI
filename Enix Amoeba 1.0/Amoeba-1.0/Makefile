all: optimised

debug: 
#	gcc -ggdb -Wall -pedantic -c Amoeba.c
	gcc -ggdb -Wall -pedantic -c Amoeba-API.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Colony.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Cytoskelenton.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Daemon.c
#	gcc -ggdb -Wall -pedantic -c Amoeba-File.c
#	gcc -ggdb -Wall -pedantic -c Amoeba-FS.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Microtubules.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Nucleus.c
#	gcc -ggdb -Wall -pedantic -c Amoeba-Nuke.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Test.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Unit-Tests.c
	gcc -ggdb -Wall -pedantic -c ENiX_LIST.c
	gcc -ggdb -Wall -pedantic -c ENiX_STRING.c
	gcc -ggdb -Wall -pedantic -c ENiX_WMS.c
#	gcc -ggdb -Wall -pedantic -c WitchCraft.c
	gcc -ggdb -Wall -pedantic -c WitchCraft-Net.c
#	gcc -ggdb ENiX_LIST.o ENiX_STRING.o Amoeba-Colony.o Amoeba-Cytoskelenton.o Amoeba-Nucleus.o Amoeba-Microtubules.o WitchCraft-Net.o Amoeba-Unit-Tests.o Amoeba.o ENiX_WMS.o Amoeba-API.o -o Amoeba -lpthread
	gcc -ggdb Amoeba-Colony.o Amoeba-Cytoskelenton.o Amoeba-Daemon.o Amoeba-Nucleus.o Amoeba-Unit-Tests.o WitchCraft-Net.o ENiX_LIST.o ENiX_WMS.o ENiX_STRING.o Amoeba-Microtubules.o Amoeba-API.o -o Daemon -lpthread
#	gcc -ggdb Amoeba-File.o ENiX_WMS.o Amoeba-Colony.o Amoeba-Cytoskelenton.o Amoeba-FS.o Amoeba-Nucleus.o Amoeba-Unit-Tests.o WitchCraft-Net.o ENiX_LIST.o ENiX_STRING.o Amoeba-Microtubules.o Amoeba-API.o -o Amoeba-FS -lpthread
	gcc -ggdb Amoeba-Test.o WitchCraft-Net.c ENiX_WMS.o Amoeba-Colony.o Amoeba-Cytoskelenton.o Amoeba-Nucleus.o Amoeba-Unit-Tests.o ENiX_LIST.o ENiX_STRING.o Amoeba-Microtubules.o Amoeba-API.o -o Amoeba-DEBUG -lpthread
#	gcc -ggdb Amoeba-Colony.o ENiX_STRING.o Amoeba-Cytoskelenton.o Amoeba-Nuke.o -o Nuke -lpthread
#	gcc -ggdb -o SHELL2.bin Amoeba-Colony.o Amoeba-Cytoskelenton.o WitchCraft.o WitchCraft-Net.o -lcurses -lreadline -lpthread
#	gcc -ggdb -c main.c
#	gcc -ggdb WitchCraft-Colours.c -o WitchCraft-Colours
#	gcc -ggdb ENiX_LIST.o ENiX_STRING.o ENiX_WMS.o main.o -o ENiX3 -lm

optimised: 
	gcc -O3 -c Amoeba-API.c
	gcc -O3 -c Amoeba-Colony.c
	gcc -O3 -c Amoeba-Cytoskelenton.c
	gcc -O3 -c Amoeba-Daemon.c
	gcc -O3 -c Amoeba-Microtubules.c
	gcc -O3 -c Amoeba-Nucleus.c
	gcc -O3 -c Amoeba-Test.c
	gcc -O3 -c Amoeba-Unit-Tests.c
	gcc -O3 -c ENiX_LIST.c
	gcc -O3 -c ENiX_STRING.c
	gcc -O3 -c ENiX_WMS.c
	gcc -O3 -c WitchCraft-Net.c
	gcc -O3 Amoeba-Colony.o Amoeba-Cytoskelenton.o Amoeba-Daemon.o Amoeba-Nucleus.o Amoeba-Unit-Tests.o WitchCraft-Net.o ENiX_LIST.o ENiX_WMS.o ENiX_STRING.o Amoeba-Microtubules.o Amoeba-API.o -o Daemon -lpthread
	gcc -O3 Amoeba-Test.o WitchCraft-Net.c ENiX_WMS.o Amoeba-Colony.o Amoeba-Cytoskelenton.o Amoeba-Nucleus.o Amoeba-Unit-Tests.o ENiX_LIST.o ENiX_STRING.o Amoeba-Microtubules.o Amoeba-API.o -o Amoeba-DEBUG -lpthread
	@echo ""
	@echo "The following binaries have been built:"
	@echo "1) ./Daemon"
	@echo "2) ./Amoeba-DEBUG"
	@echo ""

clean:
	rm -f *.o
	rm -f *~
	rm -f Amoeba-DEBUG
	rm -f Daemon
	rm -f Data1.ENiX
	rm -f Data2.ENiX
	@echo ""
	@echo "All old files removed."
	@echo ""
