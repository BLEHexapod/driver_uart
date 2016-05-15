/* 
 * Copyright 2015 - 2016 Bart Monhemius.
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

#ifndef PINDEFS_H
#define	PINDEFS_H

#include <p32xxxx.h>
#include <inttypes.h>
#include <sys/attribs.h>
#include <sys/types.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
typedef volatile unsigned int *PORTREF;
typedef void(*callback)(void*);
typedef void* bundle;


// system clock(Cerebot stock crystal) CPU
#ifndef CPUCLK
#define CPUCLK      8000000
#endif // CPUCLK
    
#ifndef SYS_CLK_FREQ
#define SYS_CLK_FREQ      64000000
#endif //SYS_CLK_FREQ

// leds
#define LEDS 4    

#define LED1 PORTBbits.RB10
#define LED2 PORTBbits.RB11
#define LED3 PORTBbits.RB12
#define LED4 PORTBbits.RB13
    
#define rB10 10
#define rB11 11
#define rB12 12
#define rB13 13

// buttons
#define BTN1 PORTAbits.RA6
#define BTN2 PORTAbits.RA7

#define rA6 6
#define rA7 7
    
    // switches
#define rF4         4
#define rF5         5
#define rF12        12
#define rF13        13
    
#define MODE_SWTCH  PORTFbits.RF5
#define ALARM_SWTCH PORTFbits.RF4    

// timer 1
#define TMRA_ON     15
#define TMRA_PS0    4
#define TMRA_PS1    5
    
// timer 2
#define TMRB_ON     15
#define TMRB_PS0    4
#define TMRB_PS1    5
#define TMRB_PS2    6
    
// Output compare 1
//#define OC_ON      15
#define OC_OCM0    0
#define OC_OCM1    1
#define OC_OCM2    2
// pins in PORTD (DO NOT TOUCH!!)
#define OC1        0
#define OC2        1
#define OC3        2
#define OC4        3
#define OC5        4
    
// ADxCON1
#define AD_ON      15
// output format uint8_t, uin16_t etc..
#define AD_FORM2   10
#define AD_FORM1   9
#define AD_FORM0   8
#define AD_SAMP    1
// ADxCON2
// Conversion trigger source
#define AD_SSRC2   7
#define AD_SSRC1   6
#define AD_SSRC0   5
// High when AD conversion is done
#define AD_DONE    0
// ADC reference voltage
#define AD_VCFG2   15
#define AD_VCFG1   14
#define AD_VCFG0   13
/* ADxCON3
 * ADC clock selection bit 7-0
 * ADC auto-sample time bit 12-8
 */
// ADC port config for AD1PCFG
#define AN1 0 
#define AN2 1 
#define AN3 2 
#define AN4 3 
#define AN5 4 
#define AN6 5 
#define AN7 6 
#define AN8 7 
#define AN9 8 
#define AN10 9 
#define AN11 10 
#define AN12 11 
#define AN13 12 
#define AN14 13 
#define AN15 14 
#define AN16 15

//UART
#define UART_ON 15
#define PDSEL0  1
#define PDSEL1  2
//#define BRGH    3
#define uRXEN   12
#define uTXEN   10
//#define URXISEL_FULL    3

// Interrupt helper present in </sys/attributes.h>


#ifdef	__cplusplus
}
#endif

#endif	/* PINDEFS_H */

