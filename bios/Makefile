
all:
	xa kim_bios.asm
	bin2c -i a.o65 -o kim_bios.h -a bios
	rm a.o65
	xa osi_bios.asm
	bin2c -i a.o65 -o osi_bios.h -a bios
	rm a.o65

clean:
	rm -f kim_bios.h osi_bios.h
