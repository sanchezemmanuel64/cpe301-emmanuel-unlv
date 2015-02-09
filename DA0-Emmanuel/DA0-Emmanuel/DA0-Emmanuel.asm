/*
 * DA0_Emmanuel.asm
 *
 *  Created: 2/8/2015 3:42:44 PM
 *   Author: Emmanuel
 */ 

	LDI R20, 48				;load 1st number
	LDI R21, 59				;load 2nd number
	ADD R20, R21			;add first 2 numbers. R20 = sum

	LDI R21, 47				;load 3rd number
	ADD R20, R21			;add 3rd number to sum

	LDI R21, 55				;load 4th number
	ADD R20, R21			;add 4th number to sum

	LDI R21, 54				;load 5th number
	ADD R20, R21			;add 5th number to sum

	BRCC noOverflow			;branch to "noOverflow" if carry bit in status reg is 0
	SBI PORTB, 2			;overflow detected: set pin 2 on port B to 1
	jmp done

noOverflow:
	CBI PORTB, 2			;no overflow detected: set pin 2 on port B to 0
done:





