;
; DA2A_part2.asm
;
; Created: 9/30/2019 9:22:22 PM
; Author : ANDREW BUCHANAN
;
; GPIOs_Part2_ASM.asm
.org 0

cbi	DDRC, 3		; set PORTC.3 to be an input
sbi	PORTC, 3	; set PORTC.3 to be active high
sbi	DDRB, 2	    ; set PORTB.3 to be an output output
sbi PORTB, 2	; set LED off
FUNC:
sbic PINC, 3	; if PINC is low go to line 17
jmp	SKIP		; jump to pin C
sbi PORTB, 2	; turn LED on

; Delay 21 328 000 cycles
; 1s 333ms at 16 MHz

    ldi  r18, 109
    ldi  r19, 51
    ldi  r20, 106
L1: dec  r20
    brne L1
    dec  r19
    brne L1
    dec  r18
    brne L1


jmp FUNC
SKIP:
Cbi	PORTB, 2	; turn LED off

jmp FUNC
