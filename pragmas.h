/* 
 * Copyright 2015 Bart Monhemius.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PRAGMAS_H
#define	PRAGMAS_H

#ifdef	__cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/* pragma's for configuring the Cerebot board.                                */
/* -------------------------------------------------------------------------- */
#pragma config ICESEL   = ICS_PGx2		// ICE/ICD Comm Channel Select
#pragma config BWP      = OFF			// Boot Flash Write Protect
#pragma config CP       = OFF			// Code Protect
#pragma config FNOSC    = PRIPLL		// Oscillator Selection
#pragma config FSOSCEN  = OFF			// Secondary Oscillator Enable
#pragma config IESO     = OFF			// Internal/External Switch-over
#pragma config POSCMOD  = HS			// Primary Oscillator
#pragma config OSCIOFNC = OFF			// CLKO Enable
#pragma config FPBDIV   = DIV_8			// Peripheral Clock divisor
#pragma config FCKSM    = CSDCMD		// Clock Switching & Fail Safe Clock Monitor
#pragma config WDTPS    = PS1			// Watchdog Timer Postscale
#pragma config FWDTEN   = OFF			// Watchdog Timer 
#pragma config FPLLIDIV = DIV_2			// PLL Input Divider
#pragma config FPLLMUL  = MUL_16		// PLL Multiplier
#pragma config UPLLIDIV = DIV_2			// USB PLL Input Divider
#pragma config UPLLEN   = OFF			// USB PLL Enabled
#pragma config FPLLODIV = DIV_1			// PLL Output Divider
#pragma config PWP      = OFF			// Program Flash Write Protect
#pragma config DEBUG    = OFF			// Debugger Enable/Disable

#ifdef	__cplusplus
}
#endif

#endif	/* PRAGMAS_H */

