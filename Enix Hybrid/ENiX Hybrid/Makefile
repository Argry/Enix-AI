
all: optimised

clean:
	rm -f *~ core a.out Recurrence.bin Omega.bin Poly.bin ENiXHybrid1.bin Test.bin SeqGen.bin IdentGen.bin *.o

debug:
	gcc -ggdb -Wall -pedantic -c ENiX_Recursion.c
	gcc -ggdb -Wall -pedantic -c ENiX_Chaos.c
	gcc -ggdb -Wall -pedantic -c ENiX_Debug.c
	gcc -ggdb -Wall -pedantic -c ENiX_Hybrid.c
	gcc -ggdb -Wall -pedantic -c ENiX_Input.c
	gcc -ggdb -Wall -pedantic -c ENiX_Maths.c
	gcc -ggdb -Wall -pedantic -c ENiX_Omega.c
	gcc -ggdb -Wall -pedantic -c ENiX_Poly.c
	gcc -ggdb -Wall -pedantic -c ENiX_Print.c
	gcc -ggdb -Wall -pedantic -c ENiX_Statistics.c
	gcc -ggdb -Wall -pedantic -c ENiX_Vector.c
	gcc -ggdb -Wall -pedantic -c Hybrid.c
	gcc -ggdb -Wall -pedantic -c IdentGen.c
	gcc -ggdb -Wall -pedantic -c Omega.c
	gcc -ggdb -Wall -pedantic -c Poly.c
	gcc -ggdb -Wall -pedantic -c Recurrence.c
	gcc -ggdb -Wall -pedantic -c SeqGen.c
	gcc -ggdb -Wall -pedantic Poly.o ENiX_Print.o ENiX_Poly.o ENiX_Input.o ENiX_Maths.o ENiX_Vector.o -o Poly.bin
	gcc -ggdb -Wall -pedantic Recurrence.o ENiX_Print.o ENiX_Vector.o ENiX_Input.o ENiX_Recursion.o -o Recurrence.bin
	gcc -ggdb -Wall -pedantic Omega.o ENiX_Vector.o ENiX_Omega.o ENiX_Input.o -o Omega.bin
	gcc -ggdb -Wall -pedantic Hybrid.o ENiX_Hybrid.o ENiX_Input.o ENiX_Chaos.o ENiX_Print.o ENiX_Vector.o ENiX_Poly.o ENiX_Recursion.o ENiX_Maths.o -o ENiXHybrid1.bin
	gcc -ggdb -Wall -pedantic SeqGen.o ENiX_Input.o ENiX_Print.o ENiX_Vector.o ENiX_Hybrid.o ENiX_Poly.o ENiX_Chaos.o ENiX_Recursion.o ENiX_Maths.o -o SeqGen.bin
	gcc -ggdb -Wall -pedantic IdentGen.o ENiX_Chaos.o ENiX_Print.o ENiX_Hybrid.o ENiX_Poly.o ENiX_Maths.o ENiX_Recursion.o ENiX_Vector.o -o IdentGen.bin
	@echo ""
	@echo "Compilation successful."
	@echo "Created binaries: IdentGen.bin, SeqGen.bin, ENiXHybrid1.bin, Omega.bin, Recurrence.bin, Poly.bin"
	@echo ":)"
	@echo ""

optimised:
	gcc -O3 -c ENiX_Recursion.c
	gcc -O3 -c ENiX_Chaos.c
	gcc -O3 -c ENiX_Debug.c
	gcc -O3 -c ENiX_Hybrid.c
	gcc -O3 -c ENiX_Input.c
	gcc -O3 -c ENiX_Maths.c
	gcc -O3 -c ENiX_Omega.c
	gcc -O3 -c ENiX_Poly.c
	gcc -O3 -c ENiX_Print.c
	gcc -O3 -c ENiX_Statistics.c
	gcc -O3 -c ENiX_Vector.c
	gcc -O3 -c Hybrid.c
	gcc -O3 -c IdentGen.c
	gcc -O3 -c Omega.c
	gcc -O3 -c Poly.c
	gcc -O3 -c Recurrence.c
	gcc -O3 -c SeqGen.c
	gcc -O3 Poly.o ENiX_Print.o ENiX_Poly.o ENiX_Input.o ENiX_Maths.o ENiX_Vector.o -o Poly.bin
	gcc -O3 Recurrence.o ENiX_Print.o ENiX_Vector.o ENiX_Input.o ENiX_Recursion.o -o Recurrence.bin
	gcc -O3 Omega.o ENiX_Vector.o ENiX_Omega.o ENiX_Input.o -o Omega.bin
	gcc -O3 Hybrid.o ENiX_Hybrid.o ENiX_Input.o ENiX_Chaos.o ENiX_Print.o ENiX_Vector.o ENiX_Poly.o ENiX_Recursion.o ENiX_Maths.o -o ENiXHybrid1.bin
	gcc -O3 SeqGen.o ENiX_Input.o ENiX_Print.o ENiX_Vector.o ENiX_Hybrid.o ENiX_Poly.o ENiX_Chaos.o ENiX_Recursion.o ENiX_Maths.o -o SeqGen.bin
	gcc -O3 IdentGen.o ENiX_Chaos.o ENiX_Print.o ENiX_Hybrid.o ENiX_Poly.o ENiX_Maths.o ENiX_Recursion.o ENiX_Vector.o -o IdentGen.bin
	@echo ""
	@echo "Compilation successful."
	@echo "Created binaries: IdentGen.bin, SeqGen.bin, ENiXHybrid1.bin, Omega.bin, Recurrence.bin, Poly.bin"
	@echo ":)"
	@echo ""