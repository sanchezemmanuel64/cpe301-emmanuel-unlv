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

LDI R28, LOW(RAMEND)	;low byte of ramend
LDI R29, HIGH(RAMEND)	;high byte of ramend

LDI R30, 0x60	;low byte of rambegin
LDI R31, 0x00	;high byte of rambegin

ADC R28, R30	;low
ADC R29, R31	;high

LSR R28
ASR R29

















