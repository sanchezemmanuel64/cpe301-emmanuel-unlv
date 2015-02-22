/*
 * DA1.asm
 *
 *  Created: 2/21/2015 12:22:29 AM
 *   Author: Emmanuel Sanchez
 *  CPE 301

Program tasks: 
--Store 25 numbers starting from the RAMEND location. Capture the lower 8bits of
the RAM MIDDLE = (RAMBEGIN + RAMEND)/2 address and use them as your
values. You can increment or decrement from RAM MIDDLE location to get the
subsequent 24 numbers. Make sure the sum is not greater than 16-bits. Use the
X/Y/Z registers to fill up the stack of 25 numbers.

--Parse through the 25 numbers and add all numbers divisible by 7 and place the
result in R20:21.

--Parse through the 25 numbers and add all numbers divisible by 3 and place the
result in R23:24. Parsing of the numbers for task b and c has to be done
simultaneously.
*/ 
.INCLUDE "m328pdef.inc"

LDI R25, 25		;load 25 as counter
LDI R24, 0		;zero register

LDI YL, LOW(RAMEND)	;low byte of ramend
LDI YH, HIGH(RAMEND) ;high byte of ramend

LDI ZL, 0x60	;low byte of rambegin
LDI ZH, 0x00	;high byte of rambegin

ADC YL, ZL	;add low byte (ramend + rambegin)
ADC YH, ZH	;add high byte

LSR YH		;divide by 2
ROR YL		;Y = ram middle = (rambegin + ramend)/2

;clear registers to store sum of numbers divisible by 7
LDI XL, 0	;low
LDI XH, 0	;high

;clear registers to store sum of numbers divisible by 3
LDI ZL, 0	;low
LDI ZH, 0	;high

loop: 
MOV R16, YL ;copy contents of YL into R16
ST Y, R16   ;copy contents of R16 into location pointed by Y

MOV R17, R16 ;copy R16 to check if div by 7
divBy7:
CPI R17, 0 ;check if remainder is 0 (means number is divisible by 7)
BREQ doneRemainder7 ;skip next subtraction if remainder is 0
SUBI R17, 7	 ;subtract 7
BRCC divBy7 ;branch if > 0 
doneRemainder7:

CPI R17, 0 ;check if remainder is 0 (div by 7)
BRNE notBy7 ;skip addition if number not divisible by 7
ADC XL, R16 ;add number to sum of numbers divisible by 7
ADC XH, r24	;add carry if any
notBy7:

MOV R17, R16 ;copy R16 to check if div by 3
divBy3:
CPI R17, 0   ;check if remainder is 0 (means number is divisible by 3)
BREQ doneRemainder3 ;skip next subtraction if remainder is 0
SUBI R17, 3  ;subtract 3
BRCC divBy3  ;branch if > 0
doneRemainder3:

CPI R17, 0	;check if remainder is 0 (div by 3)
BRNE notBy3 ;skip addition if number not divisible by 3
ADC ZL, R16	;add number to sum of numbers divisible by 3
ADC ZH, R24	;add carry if any
notBy3:

ADIW YH:YL, 1	;point to next location in ram
DEC R25			;decrement counter
CPI R25, 0		;stops looping after storing 25 numbers
BRNE loop		;loop back for next number

ADIW XH:XL, 0	;pause here to check for overflow
BRCS overflow7	;check carry for overflow
MOV R20, XL		;if no overflow, save sum of numbers div by 7 into R21:R20
MOV R21, XH
overflow7:


ADIW ZH:ZL, 0	;pause here to check for overflow
BRCS overflow3	;check carry for overflow
MOV R23, ZL		;if no overflow, save sum of numbers div by 3 into R24:R23
MOV R24, ZH
overflow3:




















