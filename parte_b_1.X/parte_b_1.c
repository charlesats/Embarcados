/*
 * File:   Rx_Interrupt.c
 * Author: Marcelo
 *
 * Created on 08 de Abril de 2021, 11:12
 */
// CONFIG
#pragma config FOSC = XT        // XT oscillator
#pragma config WDTE = OFF       // WDT disabled
#pragma config PWRTE = OFF      // PWRT disabled
#pragma config BOREN = OFF      // BOR disabled
#pragma config LVP = OFF        // RB3 is digital I/O, HV on MCLR must be used for programming
#pragma config CPD = OFF        // Data EEPROM code protection off
#pragma config WRT = OFF        // Write protection off; all program memory may be written to by EECON control
#pragma config CP = OFF         // Code protection off

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 4000000 // 4MHz
#define LED0 PORTBbits.RB0


void setStringOnSerial(char * outputString){
    int contador=0;
    while(outputString[contador] != 0){
        TXREG = outputString[contador];                
        contador++;                             
        while(!PIR1bits.TXIF);                      
    }
}

void portsObserver(){
    if(!PORTBbits.RB0){
        setStringOnSerial("S1 pressionada\n");
        while(1){
            if(PORTBbits.RB0){
                setStringOnSerial("S1 liberada\n");
                break;
            }
        }
      }
      if(!PORTBbits.RB1){
          setStringOnSerial("S2 pressionada\n");
          while(1){
              if(PORTBbits.RB1){
                  setStringOnSerial("S2 liberada\n");
                  break;
              }
          }
      }
}


void main(void) {    

     // UART
    TXSTAbits.TX9 = 0; // 0 = Selects 8-bit transmission
    TXSTAbits.TXEN = 1; // 1 = Transmit enabled
    TXSTAbits.SYNC = 0; // 0 = Asynchronous mode
    TXSTAbits.BRGH = 1; // 1 = High speed

    RCSTAbits.SPEN = 1; // 1 = Serial port enabled (configures RC7/RX/DT and RC6/TX/CK pins as serial port pins)
    
    // RX
    INTCONbits.PEIE = 1; // Peripheral Interrupt Enable bit
    INTCONbits.GIE = 1;  // Global Interrupt Enable bit
    PIE1bits.RCIE = 1;   // 1 = Enables the USART receive interrupt
    
        // LEDs
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    
   

    
    while(1){
        TRISB=0x0F;
    
        portsObserver();
        
        PORTB=0x00;
        TRISB=0x00;
    }    
}