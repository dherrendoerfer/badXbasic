; Fake BIOS 
*=$FF10
CHRIN
                lda $FF01
                beq     no_keypressed
                ;jsr     CHROUT                  ; echo

                sec
                rts
no_keypressed
                clc
                rts

CHROUT
                sta $FF00
                rts

.dsb $FFEB - *
MONRDKEY
                jmp CHRIN

.dsb $FFEE - *          
MONCOUT
                jmp CHROUT

.dsb $FFF1 - *
MONISCNTC
                rts

