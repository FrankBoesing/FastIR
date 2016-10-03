/*
	FastIR library

	Copyright (c) 2014, 2016 Frank Bösing

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Dieses Programm ist Freie Software: Sie können es unter den Bedingungen
	der GNU General Public License, wie von der Free Software Foundation,
	Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
	veröffentlichten Version, weiterverbreiten und/oder modifizieren.

	Dieses Programm wird in der Hoffnung, dass es nützlich sein wird, aber
	OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
	Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
	Siehe die GNU General Public License für weitere Details.

	Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
	Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
 */


#if (!defined(FastIR_h)) && defined(KINETISK)
#define FastIR_h

#include <kinetis.h>
#include <inttypes.h>

////////////////////////////////////////////////////////////////
// Tunable parameters
////////////////////////////////////////////////////////////////

//Select ONE of these pins:

//#define IR_PIN  3 // FTM1_CH0
//#define IR_PIN  4 // FTM1_CH1
#define IR_PIN  5 // FTM0_CH7
//#define IR_PIN  6 // FTM0_CH4
//#define IR_PIN  9 // FTM0_CH2
//#define IR_PIN 10 // FTM0_CH3
//#define IR_PIN 16 // FTM1_CH0
//#define IR_PIN 17 // FTM1_CH1
//#define IR_PIN 20 // FTM0_CH5
//#define IR_PIN 21 // FTM0_CH6
//#define IR_PIN 22 // FTM0_CH0
//#define IR_PIN 23 // FTM0_CH1
//#define IR_PIN 24 // FTM0_CH2
//#define IR_PIN 25 // FTM2_CH1
//#define IR_PIN 32 // FTM2_CH0
//#define IR_PIN 33 // FTM0_CH1

//Overview of Protocols:
//see http://www.sbprojects.com/knowledge/ir/index.php

//If you know the protocol, set these numbers close to number of bits the procotol uses:
//Example Sony Sirc: 12-20 Bits, Bitlen : max 1.8ms, Time between Commands: 45 ms

//These defaults should work with most protocols:

#define IR_MINBITS		6
#define IR_MAXBITS		40
#define IR_TIMEOUT_MS	20	 // Timeout in milliseconds (ms)


////////////////////////////////////////////////////////////////
// Do *NOT* change:
////////////////////////////////////////////////////////////////


class FastIR {
public:
	FastIR();
	unsigned int getkeypress();
};

//Clock source is set to Fixed Frequency Clock running at 31.25kHz (FLL Clock input = MCGFFCLK)
//Advantage: independent from F_CPU
// See: mk20dx128.c :
//		switch to crystal as clock source, FLL input = 16 MHz / 512
//		MCG_C1 =  MCG_C1_CLKS(2) | MCG_C1_FRDIV(4);

#define IR_FTM_CLKS	  ((1<<4) | (0<<3)) // select FLL Clock input
// Prescale: Clock / 2 = 15.624 kHz
#define IR_FTM_CLKPS  0x00;
#define IR_TIMEOUT	  ( IR_TIMEOUT_MS * 31250 / (1<<(IR_FTM_CLKPS+1)) / 1000)




#if IR_PIN == 3
 #define IR_PINCONFIG PORTA_PCR12 = 0x310
 #define IR_FTM_CV	  FTM1_C0V
 #define IR_FTM_CSC	  FTM1_C0SC
#elif IR_PIN == 4
 #define IR_PINCONFIG PORTA_PCR13 = 0x310 
 #define IR_FTM_CV	  FTM1_C1V
 #define IR_FTM_CSC	  FTM1_C1SC
#elif IR_PIN == 5
 #define IR_PINCONFIG PORTD_PCR7  = 0x410
 #define IR_FTM_CV	  FTM0_C7V
 #define IR_FTM_CSC	  FTM0_C7SC
#elif IR_PIN == 6
 #define IR_PINCONFIG PORTD_PCR4  = 0x410
 #define IR_FTM_CV	  FTM0_C4V
 #define IR_FTM_CSC	  FTM0_C4SC
#elif IR_PIN == 9
 #define IR_PINCONFIG PORTC_PCR3  = 0x410
 #define IR_FTM_CV	  FTM0_C2V
 #define IR_FTM_CSC	  FTM0_C2SC
#elif IR_PIN == 10
 #define IR_PINCONFIG PORTC_PCR4  = 0x410
 #define IR_FTM_CV	  FTM0_C3V
 #define IR_FTM_CSC	  FTM0_C3SC
#elif IR_PIN == 16
 #define IR_PINCONFIG PORTB_PCR0  = 0x310
 #define IR_FTM_CV	  FTM1_C0V
 #define IR_FTM_CSC	  FTM1_C0SC
#elif IR_PIN == 17
 #define IR_PINCONFIG PORTB_PCR1  = 0x310
 #define IR_FTM_CV	  FTM1_C1V
 #define IR_FTM_CSC	  FTM1_C1SC
#elif IR_PIN == 20
 #define IR_PINCONFIG PORTD_PCR5  = 0x410
 #define IR_FTM_CV	  FTM0_C5V
 #define IR_FTM_CSC	  FTM0_C5SC
#elif IR_PIN == 21
 #define IR_PINCONFIG PORTD_PCR6  = 0x410
 #define IR_FTM_CV	  FTM0_C6V
 #define IR_FTM_CSC	  FTM0_C6SC
#elif IR_PIN == 22
 #define IR_PINCONFIG PORTC_PCR1  = 0x410
 #define IR_FTM_CV	  FTM0_C0V
 #define IR_FTM_CSC	  FTM0_C0SC
#elif IR_PIN == 23
 #define IR_PINCONFIG PORTC_PCR2  = 0x410
 #define IR_FTM_CV	  FTM0_C1V
 #define IR_FTM_CSC	  FTM0_C1SC
#elif IR_PIN == 24
 #define IR_PINCONFIG PORTA_PCR5  = 0x310
 #define IR_FTM_CV	  FTM0_C2V
 #define IR_FTM_CSC	  FTM0_C2SC
#elif IR_PIN == 25
 #define IR_PINCONFIG PORTB_PCR19 = 0x310
 #define IR_FTM_CV	  FTM2_C1V
 #define IR_FTM_CSC	  FTM2_C1SC
#elif IR_PIN == 32
 #define IR_PINCONFIG PORTB_PCR18 = 0x310
 #define IR_FTM_CV	  FTM2_C0V
 #define IR_FTM_CSC	  FTM2_C0SC
#elif IR_PIN == 33
 #define IR_PINCONFIG PORTA_PCR4  = 0x310
 #define IR_FTM_CV	  FTM0_C1V
 #define IR_FTM_CSC	  FTM0_C1SC
#else
 #error selected pin not allowed
#endif




#if IR_PIN == 3 || IR_PIN == 4 || IR_PIN == 16 || IR_PIN == 17

 #define IR_FTM_MODE   FTM1_MODE
 #define IR_FTM_FILTER FTM1_FILTER
 #define IR_FTM_SC	   FTM1_SC
 #define IR_FTM_IRQ	   IRQ_FTM1
 #define IR_FTM_ISR	   ftm1_isr


#elif IR_PIN == 25 || IR_PIN == 32

 #define IR_FTM_MODE   FTM2_MODE
 #define IR_FTM_FILTER FTM2_FILTER
 #define IR_FTM_SC	   FTM2_SC
 #define IR_FTM_IRQ	   IRQ_FTM2
 #define IR_FTM_ISR	   ftm2_isr

#else

 #define IR_FTM_MODE   FTM0_MODE
 #define IR_FTM_FILTER FTM0_FILTER
 #define IR_FTM_SC	   FTM0_SC
 #define IR_FTM_IRQ	   IRQ_FTM0
 #define IR_FTM_ISR	   ftm0_isr

#endif




#else
#error	This microcontroller is currently not supported.
#endif
