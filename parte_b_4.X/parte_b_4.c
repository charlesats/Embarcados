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
#define LED2 PORTBbits.RB2
#define LED3 PORTBbits.RB3

unsigned char input;
unsigned char serialFlag = 0;
unsigned int aux;

void setStringOnSerial(char * outputString){
    int count=0;
    while(outputString[count] != 0){
        TXREG = outputString[count];                
        count++;                             
        while(!PIR1bits.TXIF);                      
    }
}

void __interrupt() UART_RX_INTERRUPTION() {
    if (PIR1bits.RCIF) {
        // Flag bit RCIF is a read-only bit which is cleared by the hardware. 
        // It is cleared when the RCREG register has been read and is empty.

        // Rx Error check
        if (RCSTAbits.FERR || RCSTAbits.OERR) // FERR: Framing Error bit, OERR: Overrun Error bit
        {
            RCSTAbits.CREN = 0; // 0 = Disables continuous receive
            RCSTAbits.CREN = 1; // 1 = Enables continuous receive
            LED2 = 1;
        }

        input = RCREG;
        serialFlag = 1;
    }
    
    if(TMR0IF==1){
        TMR0IF=0;       // clear T0IF
        TMR0 = 0x05;    // TMR0 value
    }
}

void handleLedSelection(){
    switch(input){
            case 'b':
                LED2 = 0;
                setStringOnSerial("\nL3 apagado");
                break;
                
            case 'a':
                LED2 = 1;
                setStringOnSerial("\nL3 aceso");
                break;
                
            case 'd':
                LED3 = 0;
                setStringOnSerial("\nL4 apagado");
                break;
            
            case 'c':
                LED3 = 1;
                setStringOnSerial("\nL4 aceso");
                break;

            default:
                setStringOnSerial("\ncodigo digito invalido");
                setStringOnSerial("\n\n");
         }
}

void main(void) {
    
    // LEDs
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    
    //TIMER0
    OPTION_REG = 0x01;  // prescaler 1:4
    // FOUT = (FOSC/4)/PRESCALER/(0xFF-TMR0)
    // TOUT = 1/FOUT
    TMR0 = 0x05;        // TMR0 value
    INTCONbits.TMR0IE=1;           //Enable timer interrupt bit in PIE1 register

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

    // Rx Interrupt
    INTCONbits.PEIE = 1; // Peripheral Interrupt Enable bit
    INTCONbits.GIE = 1; // Global Interrupt Enable bit
    PIE1bits.RCIE = 1; // 1 = Enables the USART receive interrupt
    
    
    while(1) {
        serialFlag=0;
        
        setStringOnSerial("\nUse L3 = a | L4 = c para acender");
        setStringOnSerial("\nUse L3 = b | L4 = d para apagar\n");
        
        while(1){
            if(serialFlag){ 
                break;
            }
        }
        
        handleLedSelection();
        
         __delay_ms(2000);
    }
 }