;
; DA1.asm
;
; Created: 9/17/2019 5:43:47 PM
; Author : bugul
;


; Replace with your application code

.org 0				;starts at 0
ldi R22, 0xff		;load lower 8 bits of multiplier
ldi R23, 0xff		;load upper 8 bits of multiplier
ldi R24, 0xff		;load lower 8 bits of multiplicand
ldi R25, 0xff	    ;load upper 8 bits of multiplicand
ldi R17, 0			;set value as 0
ldi R18, 0			;set value as 0
ldi R19, 0			;set value as 0
ldi R20, 0			;set value as 0
ldi R21, 0			;set value as 0

BLAH1:
cpi R23, 0 
breq BLAH2
add R17, R24		;add lower 8 bits into R17
adc R18, R25		;add upper 9 bits into R18 with carry
adc R19, R20		;add carry R20 into R19
subi R23,1				;decrement counter of upper 8 bits of multiplier
cpi R23, 0
breq BLAH2			;loops while counter is greater than zero
jmp BLAH1

BLAH2:
cpi R22, 0 
breq END
add R18, R24		;add lower 8 bits
adc R19, R25		;add upper 9 bits
adc R20, R21		;add carry
subi R22,1				;decrements
cpi R22, 0 
breq END		;loops while counter is greater than zero
jmp BLAH2

//answer is R20-R17
END:
jmp END			