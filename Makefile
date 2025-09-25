
CC = gcc

# Important we need to turn optimization on for the cpu driver
CFLAGS = -O3 -march=native -funroll-loops -Wall -I.. 

.PHONY: bios

all: bin bios
	$(CC) $(CFLAGS) -c -o fake6502.o -I ../badX16/cpu  ../badX16/cpu/fake6502.c #-DSTEPDEBUG
	$(CC) $(CFLAGS) -o kim1basic kim1basic.c fake6502.o -D FAST
	$(CC) $(CFLAGS) -o osibasic osibasic.c fake6502.o -D FAST

bin:
	bin2c -i ./msbasic/kb9.bin -o ./msbasic/kb9_msbasic.h -l 40 -a msbasic_bin
	bin2c -i ./msbasic/osi.bin -o ./msbasic/osi_msbasic.h -l 40 -a msbasic_bin

bios:
	make -C ./bios all

clean:
	rm -f *.o kim1basic ./msbasic/kb9_msbasic.h osibasic ./msbasic/osi_msbasic.h
	make -C ./bios clean
