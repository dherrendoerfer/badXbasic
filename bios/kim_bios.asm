; Fake BIOS 
*=$1E00
.dsb $1E5A - *
MONRDKEY
CHRIN
                lda $FF01
                beq     no_keypressed
                ;jsr     CHROUT                  ; echo

                sec
                rts
no_keypressed
                clc
                rts


.dsb $1EA0 - *
MONCOUT
CHROUT
                sta $FF00
                rts