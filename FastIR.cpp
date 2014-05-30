/*
    FastIR library

    Copyright (c) 2014 Frank Bösing

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


//
#if defined(__MK20DX128__) || defined(__MK20DX256__)

#include "FastIR.h"


#ifndef BITBAND_PERI
  #define BITBAND_PERI_REF  0x40000000
  #define BITBAND_PERI_BASE 0x42000000
  #define BITBAND_PERI(a,b) *((volatile unsigned int *) ((BITBAND_PERI_BASE + (( (uint32_t)&(a)-BITBAND_PERI_REF)*32 + (b*4))))) // Convert PERIPHAL address
#endif


static volatile uint32_t ir_pressedKey;

//////////////////////////////////////////////////////////////////////////
// ISR
#define FNV_PRIME_32 16777619
#define FNV_BASIS_32 2166136261

void IR_FTM_ISR(void)
{

static uint16_t shortestTime = 0xffff;
static uint16_t halfBitCnt = 0;
static uint16_t halfBits [IR_MAXBITS * 2];
static uint16_t lastTimervalue;
uint32_t timervalue;
uint32_t time;

    // Clear Channel Interrupt Flag
    BITBAND_PERI(IR_FTM_CSC,7) = 0;// IR_FTM_CSC &= ~0x80;

    if (BITBAND_PERI(IR_FTM_SC, 7) != 0) { //Timer-Overflow ?
        // Clear Overflow Flag
        BITBAND_PERI(IR_FTM_SC, 7) = 0; // IR_FTM_SC &= ~FTM_SC_TOF;
        timervalue = IR_FTM_CV + 0x10000;
    }
    else
        timervalue = IR_FTM_CV;

    time = timervalue - lastTimervalue;
    lastTimervalue = timervalue;

    if (time > IR_TIMEOUT)
    {

        if (halfBitCnt > (IR_MINBITS * 2))
        {

            //Normalize measured half bit-times
            uint32_t hash = FNV_BASIS_32;
            while (halfBitCnt > 0)
            {
                halfBitCnt--;
                uint8_t tmp =  (halfBits[halfBitCnt] / shortestTime);
                //FNV-Hash http://isthe.com/chongo/tech/comp/fnv/#FNV-1a
                hash = ( hash ^ tmp  ) * FNV_PRIME_32;
            }

            ir_pressedKey = hash;

        }

        else ir_pressedKey = 0;

        //reset for next received code
        halfBitCnt = 0;
        shortestTime = 0xffff;
    }

    else if (halfBitCnt < ((IR_MAXBITS * 2) - 1))
    {
        halfBits[halfBitCnt++] = (uint16_t)time;

        if ( (uint16_t)time < shortestTime)
        {
           shortestTime = (uint16_t)time;
        }
    }

}

unsigned int FastIR::getkeypress()
{
    unsigned int tmp = ir_pressedKey;
    ir_pressedKey = 0;
    return tmp;
}

FastIR::FastIR()
{
    // PIN configuration
    IR_PINCONFIG;

    //Enable FTM
    IR_FTM_MODE = 0x05;

    // select system clock, clock prescaler
    IR_FTM_SC = IR_FTM_CLKS | IR_FTM_CLKPS;

    // CHF=0 CHIE=1 MSB=0 MSA=0 ELSB=1 ELSA=1 DMA=0
    IR_FTM_CSC = 0x4C;

    // Lowest interrupt priority is sufficient: the time of the Pinchange is captured and stored in FTMx_CxV
    NVIC_SET_PRIORITY(IR_FTM_IRQ, 0xFF);

    // Enable FTM interrupt inside NVIC
    NVIC_ENABLE_IRQ(IR_FTM_IRQ);

    ir_pressedKey = 0;
}

#endif