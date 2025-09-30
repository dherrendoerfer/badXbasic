
all:
	xa kim_bios.asm
	bin2c -i a.o65 -o kim_bios.h -a bios
	rm a.o65
	xa osi_bios.asm
	bin2c -i a.o65 -o osi_bios.h -a bios
	rm a.o65
	xa cbm2_bios.asm
	bin2c -i a.o65 -o cbm2_bios.h -a bios
	rm a.o65
	xa microtan_bios.asm
	bin2c -i a.o65 -o microtan_bios.h -a bios
	rm a.o65

clean:
	rm -f kim_bios.h osi_bios.h cbm2_bios.h microtan_bios.h
