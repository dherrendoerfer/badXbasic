; Fake BIOS 
*=$FF10
BADKBDIN
                lda $FF01
                beq     no_keypressed
                ;jsr     BADTEROUT                  ; echo

                sec
                rts
no_keypressed
                clc
                rts

BADTEROUT
                sta $FF00
                rts


.dsb $FFCF - *
CHRIN
                jmp BADKBDIN

.dsb $FFD2 - *          
CHROUT
                jmp BADTEROUT

.dsb $FFE7 - *          
                rts

.dsb $FFF4 - *
GETIN
                jmp BADKBDIN

