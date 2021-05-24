/* 
 * File:   Portao_Eletronico_02.c
 * Alunos:      Charles Assis Torres Silva, 
 *              Giuliane Eulalia Correa, 
 *              Gustavo Alves Abreu,
 *              Luisabelly de Castro Oliveira 
 * 
 * Professor:   Marcelo Moreira Tiago 
 *
 * Created on 17 de Abril de 2021, 8:00
 */

// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <string.h>
#include "lcd.h"
#include "serial.h"
#include "funcoes_Portao.h"
#define _XTAL_FREQ 4000000 // 4MHz
#define LED2 PORTBbits.RB2

void main(void) {

    unsigned char leitura, i, x;
    
    // Serial
    TRISCbits.TRISC7 = 1; //RX
    TRISCbits.TRISC6 = 0; //TX
    serial_init();
    
    // LCD
    TRISD=0x00;
    TRISEbits.TRISE0=0;
    TRISEbits.TRISE1=0;
    

    iniciarMemoria();
    LED2 = 0;

    while (1) {
        leitura = serial_rx(2000); // Leitura do terminal serial
        serial_tx(leitura);
        
        if (leitura != 0) {
            lcd_init(); // Inicia o LCD
            lcd_cmd(L_CLR); // Clear LCD Display
            lcd_cmd(L_L1); // Escreve na linha 1
            lcd_str("Pressione * para"); // * Para fazer login
            serial_tx_str("\nPressione * para"); //Exibir a mensagem no terminal serial
            lcd_cmd(L_L2); // Escreve na linha 2 
            lcd_str("entrar ou #"); // # Para mudar o usuário
            serial_tx_str("\nentrar ou #"); //Exibir a mensagem no terminal serial
            __delay_ms(1500); // Delay de 1.5 segundo para não sobrepor dados na memória
            lcd_cmd(L_CLR); // Clear LCD Display
            lcd_cmd(L_L1); // Escreve na linha 1
            lcd_str("para configurar");
            serial_tx_str("\npara configurar\n"); //Exibir a mensagem no terminal serial
            

            leitura = serial_rx(2000);  // Leitura do terminal serial
            serial_tx(leitura); //Exibir a mensagem no terminal serial
            lcd_cmd(L_CLR); // Clear LCD Display
            lcd_cmd(L_L2); // Escreve na linha 1
            lcd_dat(leitura); //Exibir os numeros no display do pic
            __delay_ms(2000);
            switch (leitura) { // Primeiro Login 
                case 42:
                {
                    x = entrarPortao();  
                    if (x == 1) {
                        lcd_cmd(L_CLR); // Clear LCD Display
                        lcd_cmd(L_L1); // Escreve na linha 1
                        lcd_str("Acesso Permitido!");
                        serial_tx_str("\nAcesso Permitido!");
                        LED2 = 1;
                        __delay_ms(2000); //Delay de 2 segundo para não sobrepor dados na memória
                        lcd_cmd(L_OFF); // Desliga o LCD
                        LED2 = 0;
                    } else {
                        lcd_cmd(L_CLR); // Clear LCD Display
                        lcd_cmd(L_L1); // Escreve na linha 1
                        lcd_str("Acesso Negado!");
                        serial_tx_str("\nAcesso Negado!");
                        __delay_ms(2000); //Delay de 2 segundo para não sobrepor dados na memória
                        lcd_cmd(L_OFF); // Desliga o LCD 
                    }
                }
                break;

                case 35:
                {
                    configurarPortao(); // Insenrir novo usuário 
                }
                break;

                default:
                {
                    lcd_cmd(L_CLR); // Clear LCD Display
                    lcd_cmd(L_L1); // Escreve na linha 1
                    lcd_str("Opcao Invalida!");
                    serial_tx_str("\nOpcao invalida!");
                    __delay_ms(2000); //Delay de 2 segundo para não sobrepor dados na memória
                    lcd_cmd(L_OFF); // Desliga o LCD 
                }
            }
        }
    }
}

