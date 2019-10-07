;
; DA2B.asm
;
; Created: 10/5/2019 9:15:12 PM
; Author : bugul
;



.org 0			; location of the reset
jmp MAIN		; start the main program
.org 0x02		; location for external interrupt INT0
jmp EXT_INT		; start the external interrupt

MAIN:
ldi R20, HIGH(RAMEND)	; initialize stack
out SPH, R20
ldi R20, LOW(RAMEND)
out SPL, R20

ldi R20, 2		; make INT0 falling edge triggered
sts EICRA, R20

sbi DDRB, 3		; set PORTB.2 as output
sbi PORTB, 3	; start the PORTB.3 LED off
sbi PORTD, 3	; turn on PORTD.3


ldi R20, 1<<INT0	; enable INT0
out EIMSK, R20
sei				; set the interupt

WHILE:
sei				; set the interupt again
; generate waveform with 40% DC and .625 sec period
cbi PORTB, 3	; set LED on
; Delay 4 000 000 cycles
; 250ms at 16 MHz
    ldi  r18, 21
    ldi  r19, 75
    ldi  r20, 191
L1: dec  r20
    brne L1
    dec  r19
    brne L1
    dec  r18
    brne L1
    nop
sbi PORTB, 3	; set LED off
; Delay 6 000 000 cycles
; 375ms at 16 MHz
    ldi  r18, 31
    ldi  r19, 113
    ldi  r20, 31
L2: dec  r20
    brne L2
    dec  r19
    brne L2
    dec  r18
    brne L2
	nop
jmp	WHILE		; wait for interrupt


EXT_INT:

cbi PORTB, 3

; Delay 17 328 000 cycles
; 1s 83ms at 16 MHz

    ldi  r18, 88
    ldi  r19, 232
    ldi  r20, 170
L3: dec  r20
    brne L3
    dec  r19
    brne L3
    dec  r18
    brne L3

jmp WHILE	; reti was taking a really long time to return to the while loop so i just
			; made the interupt jump back up to the while loop then set the inerupt again