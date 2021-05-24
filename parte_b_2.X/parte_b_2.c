/*
 * File:   Teste_ADC_LCD.c
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
#include "itoa.h"
#define _XTAL_FREQ 4000000 // 4MHz
#define LED0 PORTBbits.RB0


void setStringOnSerial(char outputString[]){
    int count=0;
    while(outputString[count] != 0){
        TXREG = outputString[count];                
        count++;                             
        while(!PIR1bits.TXIF);                      
    }
}


void __interrupt() UART_RX_INTERRUPTION() {

    // Rx
    if (PIR1bits.RCIF) {
        // Flag bit RCIF is a read-only bit which is cleared by the hardware. 
        // It is cleared when the RCREG register has been read and is empty.

        // Rx Error check
        if (RCSTAbits.FERR || RCSTAbits.OERR) // FERR: Framing Error bit, OERR: Overrun Error bit
        {
            RCSTAbits.CREN = 0; // 0 = Disables continuous receive
            RCSTAbits.CREN = 1; // 1 = Enables continuous receive
            LED0 = 1;
        }
    }
    
    if(TMR0IF==1){
        TMR0IF=0;       // clear T0IF
        TMR0 = 0x05;    // TMR0 value
    }
}

void main(void) {
    unsigned int potentiometer;
    char voltageConvertedToString[6];
    char preComma = '0';
    char posComma = '0';
    
    
    // ADC CONFIG
    // TRISx
    TRISAbits.TRISA1=1;     // RA0 INPUT
    
    // A/D Conversion Clock
    ADCON1bits.ADCS2 = 1;
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADCS0 = 0;// FOSC/64
    
    // Analog Channel Select bits. (AN1-RA0) = POT2, (AN2-RA2) - Usar Gerador
    ADCON0bits.CHS = 0b001; // Channel 1 (AN1) - RA0
    
    // A/D Result Format Select bit
    ADCON1bits.ADFM = 1; // Right justified. Six (6) Most Significant bits of ADRESH are read as ?0?.
    
    // A/D Port Configuration Control bits
    ADCON1bits.PCFG = 0b0010;
    
    // A/D On bit
    ADCON0bits.ADON = 1;
    
    //TIMER0
    OPTION_REG = 0x01;  // prescaler 1:4
    TMR0 = 0x05;        // TMR0 value
    INTCONbits.TMR0IE=1;           //Enable timer interrupt bit in PIE1 register
    
    // UART Config
    TRISCbits.TRISC7 = 1; //RX
    TRISCbits.TRISC6 = 0; //TX
    
    SPBRG = 25; // Baud Rate = 9600, Error (%) = 0.16
    
    TXSTAbits.TX9 = 0; // 0 = Selects 8-bit transmission
    TXSTAbits.TXEN = 1; // 1 = Transmit enabled
    TXSTAbits.SYNC = 0; // 0 = Asynchronous mode
    TXSTAbits.BRGH = 1; // 1 = High speed

    RCSTAbits.SPEN = 1; // 1 = Serial port enabled (configures RC7/RX/DT and RC6/TX/CK pins as serial port pins)
    RCSTAbits.RX9 = 0; // 0 = Selects 8-bit reception
    RCSTAbits.CREN = 1; // 1 = Enables continuous receive

    // Rx Interrupt
    INTCONbits.PEIE = 1; // Peripheral Interrupt Enable bit
    INTCONbits.GIE = 1; // Global Interrupt Enable bit
    PIE1bits.RCIE = 1; // 1 = Enables the USART receive interrupt
    
    while(1){
        
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO == 1);
        
        potentiometer = (unsigned int) (ADRESH<<8 | ADRESL);
        
        potentiometer=potentiometer*50/1023;
        itoa(potentiometer,voltageConvertedToString); 
        
        if(voltageConvertedToString[3]!= preComma || voltageConvertedToString[4]!= posComma){
            preComma = voltageConvertedToString[3];
            posComma = voltageConvertedToString[4];
            setStringOnSerial("Tensao Mensurada: ");
            TXREG = voltageConvertedToString[3];
            setStringOnSerial(",");
            TXREG = voltageConvertedToString[4];
            setStringOnSerial(" V");
            setStringOnSerial("\n");
        }
    }
}