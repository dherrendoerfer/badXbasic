; Fake BIOS 
*=$FF10
CHRIN
                lda $FF01
                beq     no_keypressed
                ;jsr     CHROUT                  ; local echo

                sec
                rts
no_keypressed
                clc
                rts

CHROUT
                sta $FF00
                rts

CNTLCHK
                lda $FF01
                beq     CNT_END
                cmp     #$03
                bne     CNT_END
                sec
                rts
CNT_END
                clc
                rts

.dsb $FFEB - *
MONRDKEY
                jmp CHRIN

.dsb $FFEE - *          
MONCOUT
                jmp CHROUT

.dsb $FFF1 - *
MONISCNTC
                jmp CNTLCHK
