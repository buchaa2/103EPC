; DA2A.asm
;
; Created: 9/28/2019 1:18:05 AM
; Author : ANDREW BUCHANAN
;


; Replace with your application code
; GPIOs_Part1_ASM.asm
.org 0
 
sbi DDRB, 3		; set PORTB.3 as output
sbi PORTB, 3	; set LED off

; generate waveform with 40% DC and .625 sec period
BEGIN:
sbi PORTB, 3	; set LED on
rcall delay_1
cbi PORTB, 3	; set LED off
rcall delay_2
jmp BEGIN

; Delay 4 000 000 cycles
; 250ms at 16 MHz

delay_1:
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
	ret

; Delay 6 000 000 cycles
; 375ms at 16 MHz
delay_2:
    ldi  r18, 31
    ldi  r19, 113
    ldi  r20, 31
L2: dec  r20
    brne L1
    dec  r19
    brne L1
    dec  r18
    brne L2
	nop
    ret
