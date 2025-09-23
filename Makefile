
CC = gcc

# Important we need to turn optimization on for the cpu driver
CFLAGS = -O3 -march=native -funroll-loops -Wall -I.. 


all: bin bios
	$(CC) $(CFLAGS) -c -o fake6502.o -I ../badX16/cpu  ../badX16/cpu/fake6502.c #-DSTEPDEBUG
	$(CC) $(CFLAGS) -o badbasic msbasic.c fake6502.o -D FAST

bin:
	bin2c -i ./msbasic/kb9.bin -o ./msbasic/kb9_msbasic.h -l 40 -a msbasic_bin

bios:
	cd bios; make

clean:
	rm -f *.o badbasic ./msbasic/kb9_msbasic.h
	cd bios; make clean
