/*
 * File:   PWM.c
 * Author: Marcelo
 *
 * Created on 22 de Março de 2021, 11:12
 */

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 4000000 // 4MHz


void main() {
    
    TRISCbits.TRISC2 = 0;   // Configure PORTC as output(RC2-PWM1, RC1-PWM2)

    CCP1CON = 0x0F;         // Select the PWM mode.
    PR2 = 0xFF;             // PR2 = 255
    CCPR1L = 0;             // By default set the dutyCycle to 0
    T2CON = 0x01;           // prescaler 1:4
    
    // PWM frequency = (FOSC/4)/PRESCALER/(PR2+1).
    // PWM period = 1/PWM frequency
    
    TMR2ON = 1; //Start the Timer for PWM generation
    
    
    // UART Config
    TRISCbits.TRISC7 = 1; //RX
    TRISCbits.TRISC6 = 0; //TX

    // BRGH = 1 (High Speed)
    // Baud Rate = FOSC/(16 (SPBRG + 1))
    // SPBRG: Baud Rate Generator Register
    SPBRG = 25; // Baud Rate = 9600, Error (%) = 0.16

    TXSTAbits.TX9 = 0; // 0 = Selects 8-bit transmission
    TXSTAbits.TXEN = 1; // 1 = Transmit enabled
    TXSTAbits.SYNC = 0; // 0 = Asynchronous mode
    TXSTAbits.BRGH = 1; // 1 = High speed

    RCSTAbits.SPEN = 1; // 1 = Serial port enabled (configures RC7/RX/DT and RC6/TX/CK pins as serial port pins)
    RCSTAbits.RX9 = 0; // 0 = Selects 8-bit reception
    RCSTAbits.CREN = 1; // 1 = Enables continuous receive

    unsigned char entrada;
    unsigned int dutyCycle = 0;
    long int teste = 0;
    
    while (1){
        if (PIR1bits.RCIF) {
            // Flag bit RCIF is a read-only bit which is cleared by the hardware. 
            // It is cleared when the RCREG register has been read and is empty.

            // Rx Error check
            if (RCSTAbits.FERR || RCSTAbits.OERR) { // FERR: Framing Error bit, OERR: Overrun Error bit
                RCSTAbits.CREN = 0; // 0 = Disables continuous receive
                RCSTAbits.CREN = 1; // 1 = Enables continuous receive
            }
            entrada = RCREG + 0x00;
        }
        
        dutyCycle = entrada * 6.67;
        
        if(dutyCycle < 0x3FF){
            __delay_ms(500);
            
            // This 10-bit value is represented by CCPR1L:CCP1CON<5:4>
            CCPR1L = (dutyCycle & 0x3FC) >> 2;      // Mask1 - CCPR1L: First 8 MSBs 
            CCP1CON |= ((dutyCycle & 0x003) << 4);  // Mask2 - CCP1CON<5:4>: 2LSBs
            dutyCycle+=51;
        }
        else{
            dutyCycle=0;
        }
    }
}