; Fake BIOS 
*=$EF00

CHRIN
                lda $FF01
                beq     no_keypressed
                :jsr     CHROUT                  ; echo

                sec
                rts
no_keypressed
                clc
                rts

CHROUT
                sta $FF00
                rts

.dsb $EF20 - *
MONRDKEY
                jmp CHRIN

.dsb $EF23 - *
MONRDKEY2
                jmp CHRIN

.dsb $EF26 - *
MONCOUT
                jmp CHROUT

