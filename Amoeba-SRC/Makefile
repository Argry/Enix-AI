all: optimised

debug: 
	cp ../Amoeba-*.* .
	cp ../ENiX_LIST.* .
	cp ../ENiX_STRING.* .
	cp ../ENiX_WMS.* .
	cp ../*DEFS.* .
	cp ../WitchCraft-Net.* .
	cp ../WitchCraft-Colours.h .
	cp ../ENiX_CALC.h .
	cp ../ENiX_Globals.* .
	cp ../ENiX_TIME.* .
	cp ../ENiX_NLP.h .
	gcc -ggdb -Wall -pedantic -c Amoeba-API.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Colony.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Cytoskelenton.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Daemon.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Microtubules.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Nucleus.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Test.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Unit-Tests.c
	gcc -ggdb -Wall -pedantic -c Amoeba-Nuke.c
	gcc -ggdb -Wall -pedantic -c Amoeba-File.c
	gcc -ggdb -Wall -pedantic -c Amoeba-FS.c
	gcc -ggdb -Wall -pedantic -c ENiX_LIST.c
	gcc -ggdb -Wall -pedantic -c ENiX_STRING.c
	gcc -ggdb -Wall -pedantic -c ENiX_WMS.c
	gcc -ggdb -Wall -pedantic -c ENiX_TIME.c
	gcc -ggdb -Wall -pedantic -c ENiX_Globals.c
	gcc -ggdb -Wall -pedantic -c WitchCraft-Net.c
	gcc Amoeba-Colony.o Amoeba-Cytoskelenton.o Amoeba-Daemon.o Amoeba-Nucleus.o Amoeba-Unit-Tests.o WitchCraft-Net.o ENiX_LIST.o ENiX_WMS.o ENiX_STRING.o ENiX_TIME.o Amoeba-Microtubules.o Amoeba-API.o ENiX_Globals.o -o Daemon -lpthread
	gcc Amoeba-Test.o WitchCraft-Net.c ENiX_WMS.o ENiX_TIME.o Amoeba-Colony.o Amoeba-Cytoskelenton.o Amoeba-Nucleus.o Amoeba-Unit-Tests.o ENiX_LIST.o ENiX_STRING.o Amoeba-Microtubules.o Amoeba-API.o ENiX_Globals.o -o Amoeba-DEBUG -lpthread
	gcc ENiX_STRING.o Amoeba-Cytoskelenton.o Amoeba-Colony.o Amoeba-Nuke.o -o Amoeba-Nuke -lpthread
	gcc WitchCraft-Net.o ENiX_WMS.o ENiX_LIST.o ENiX_STRING.o ENiX_TIME.o Amoeba-Unit-Tests.o Amoeba-Nucleus.o Amoeba-API.o Amoeba-File.o Amoeba-Microtubules.o Amoeba-Cytoskelenton.o Amoeba-Colony.o Amoeba-FS.o -o Amoeba-FS -lpthread
	@echo ""
	@echo "The following binaries have been built:"
	@echo "1) ./Daemon"
	@echo "2) ./Amoeba-DEBUG"
	@echo "3) ./Amoeba-Nuke"
	@echo "4) ./Amoeba-FS"
	@echo ""

optimised: 
	cp ../Amoeba-*.* .
	cp ../ENiX_LIST.* .
	cp ../ENiX_STRING.* .
	cp ../ENiX_WMS.* .
	cp ../*DEFS.* .
	cp ../WitchCraft-Net.* .
	cp ../WitchCraft-Colours.h .
	cp ../ENiX_CALC.h .
	cp ../ENiX_Globals.* .
	cp ../ENiX_TIME.* .
	cp ../ENiX_NLP.h .
	gcc -O3 -Wno-write-strings -c Amoeba-API.c
	gcc -O3 -Wno-write-strings -c Amoeba-Colony.c
	gcc -O3 -Wno-write-strings -c Amoeba-Cytoskelenton.c
	gcc -O3 -Wno-write-strings -c Amoeba-Daemon.c
	gcc -O3 -Wno-write-strings -c Amoeba-Microtubules.c
	gcc -O3 -Wno-write-strings -c Amoeba-Nucleus.c
	gcc -O3 -Wno-write-strings -c Amoeba-Test.c
	gcc -O3 -Wno-write-strings -c Amoeba-Unit-Tests.c
	gcc -O3 -Wno-write-strings -c Amoeba-Nuke.c
	gcc -O3 -Wno-write-strings -c Amoeba-File.c
	gcc -O3 -Wno-write-strings -c Amoeba-FS.c
	gcc -O3 -Wno-write-strings -c ENiX_LIST.c
	gcc -O3 -Wno-write-strings -c ENiX_STRING.c
	gcc -O3 -Wno-write-strings -c ENiX_WMS.c
	gcc -O3 -Wno-write-strings -c ENiX_TIME.c
	gcc -O3 -Wno-write-strings -c ENiX_Globals.c
	gcc -O3 -Wno-write-strings -c WitchCraft-Net.c
	gcc -O3 Amoeba-Colony.o Amoeba-Cytoskelenton.o Amoeba-Daemon.o Amoeba-Nucleus.o Amoeba-Unit-Tests.o WitchCraft-Net.o ENiX_LIST.o ENiX_WMS.o ENiX_STRING.o ENiX_TIME.o Amoeba-Microtubules.o Amoeba-API.o ENiX_Globals.o -o Daemon -lpthread
	gcc -O3 Amoeba-Test.o WitchCraft-Net.c ENiX_WMS.o ENiX_TIME.o Amoeba-Colony.o Amoeba-Cytoskelenton.o Amoeba-Nucleus.o Amoeba-Unit-Tests.o ENiX_LIST.o ENiX_STRING.o Amoeba-Microtubules.o Amoeba-API.o ENiX_Globals.o -o Amoeba-DEBUG -lpthread
	gcc -O3 ENiX_STRING.o Amoeba-Cytoskelenton.o Amoeba-Colony.o Amoeba-Nuke.o -o Amoeba-Nuke -lpthread
	gcc -O3 WitchCraft-Net.o ENiX_WMS.o ENiX_LIST.o ENiX_STRING.o ENiX_TIME.o Amoeba-Unit-Tests.o Amoeba-Nucleus.o Amoeba-API.o Amoeba-File.o Amoeba-Microtubules.o Amoeba-Cytoskelenton.o Amoeba-Colony.o Amoeba-FS.o -o Amoeba-FS -lpthread
	@echo ""
	@echo "The following binaries have been built:"
	@echo "1) ./Daemon"
	@echo "2) ./Amoeba-DEBUG"
	@echo "3) ./Amoeba-Nuke"
	@echo "4) ./Amoeba-FS"
	@echo ""

g++: 
	cp ../Amoeba-*.* .
	cp ../ENiX_LIST.* .
	cp ../ENiX_STRING.* .
	cp ../ENiX_WMS.* .
	cp ../*DEFS.* .
	cp ../WitchCraft-Net.* .
	cp ../WitchCraft-Colours.h .
	cp ../ENiX_CALC.h .
	cp ../ENiX_Globals.* .
	cp ../ENiX_TIME.* .
	cp ../ENiX_NLP.h .
	g++ -O3 -Wno-write-strings -c Amoeba-API.c
	g++ -O3 -Wno-write-strings -c Amoeba-Colony.c
	g++ -O3 -Wno-write-strings -c Amoeba-Cytoskelenton.c
	g++ -O3 -Wno-write-strings -c Amoeba-Daemon.c
	g++ -O3 -Wno-write-strings -c Amoeba-Microtubules.c
	g++ -O3 -Wno-write-strings -c Amoeba-Nucleus.c
	g++ -O3 -Wno-write-strings -c Amoeba-Test.c
	g++ -O3 -Wno-write-strings -c Amoeba-Unit-Tests.c
	g++ -O3 -Wno-write-strings -c Amoeba-Nuke.c
	g++ -O3 -Wno-write-strings -c Amoeba-File.c
	g++ -O3 -Wno-write-strings -c Amoeba-FS.c
	g++ -O3 -Wno-write-strings -c ENiX_LIST.c
	g++ -O3 -Wno-write-strings -c ENiX_STRING.c
	g++ -O3 -Wno-write-strings -c ENiX_WMS.c
	g++ -O3 -Wno-write-strings -c ENiX_TIME.c
	g++ -O3 -Wno-write-strings -c ENiX_Globals.c
	g++ -O3 -Wno-write-strings -c WitchCraft-Net.c
	g++ -O3 Amoeba-Colony.o Amoeba-Cytoskelenton.o Amoeba-Daemon.o Amoeba-Nucleus.o Amoeba-Unit-Tests.o WitchCraft-Net.o ENiX_LIST.o ENiX_WMS.o ENiX_STRING.o ENiX_TIME.o Amoeba-Microtubules.o Amoeba-API.o ENiX_Globals.o -o Daemon -lpthread
	g++ -O3 Amoeba-Test.o WitchCraft-Net.c ENiX_WMS.o ENiX_TIME.o Amoeba-Colony.o Amoeba-Cytoskelenton.o Amoeba-Nucleus.o Amoeba-Unit-Tests.o ENiX_LIST.o ENiX_STRING.o Amoeba-Microtubules.o Amoeba-API.o ENiX_Globals.o -o Amoeba-DEBUG -lpthread
	g++ -O3 ENiX_STRING.o Amoeba-Cytoskelenton.o Amoeba-Colony.o Amoeba-Nuke.o -o Amoeba-Nuke -lpthread
	g++ -O3 WitchCraft-Net.o ENiX_WMS.o ENiX_LIST.o ENiX_STRING.o ENiX_TIME.o Amoeba-Unit-Tests.o Amoeba-Nucleus.o Amoeba-API.o Amoeba-File.o Amoeba-Microtubules.o Amoeba-Cytoskelenton.o Amoeba-Colony.o Amoeba-FS.o -o Amoeba-FS -lpthread
	@echo ""
	@echo "The following binaries have been built:"
	@echo "1) ./Daemon"
	@echo "2) ./Amoeba-DEBUG"
	@echo "3) ./Amoeba-Nuke"
	@echo "4) ./Amoeba-FS"
	@echo ""

clean:
	rm -f *.o
	rm -f *~
	rm -f *.c
	rm -f *.h
	rm -f Amoeba-DEBUG
	rm -f Daemon
	rm -f Data1.ENiX
	rm -f Data2.ENiX
	@echo ""
	@echo "All old files removed."
	@echo ""
