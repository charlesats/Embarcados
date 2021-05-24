/* 
 * File:   Portao_Eletronico.c
 * Alunos:      Charles Assis Torres Silva, 
 *              Giuliane Eulalia Correa, 
 *              Gustavo Alves Abreu,
 *              Luisabelly de Castro Oliveira 
 * 
 * Professor:   Marcelo Moreira Tiago 
 *
 * Created on 7 de Abril de 2021, 10:19
 */

// PIC16F628A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <string.h>
#include "lcd.h"
#include "teclado.h"
#include "funcoes_Portao.h"
#define _XTAL_FREQ 4000000 // 4MHz

void main(void) {

    unsigned char teclaX, i, x;


    CMCON = 0x07;
    TRISA = 0x30;
    TRISB = 0xE7;

    PORTA = 0xFE;
    PORTB = 0x00;

    iniciarMemoria();



    while (1) {
        teclaX = tc_tecla(3000) + 0x30; // Soma para pegar o números corretos pela tabela ASCII
        if (teclaX != 0) {
            lcd_init(); // Inicia o LCD
            lcd_cmd(L_CLR); // Clear LCD Display
            lcd_cmd(L_L1); // Escreve na linha 1
            lcd_str("Pressione * para"); // * Para fazer login 
            lcd_cmd(L_L2); // Escreve na linha 2 
            lcd_str("entrar ou #"); // # Para mudar o usuário
            __delay_ms(1500); // Delay de 1.5 segundo para não sobrepor dados na memória
            lcd_cmd(L_CLR); // Clear LCD Display
            lcd_cmd(L_L1); // Escreve na linha 1
            lcd_str("para configurar");
            

            teclaX = tc_tecla(3000) + 0x30; // Soma para pegar o números corretos pela tabela ASCII

            switch (teclaX) { // Primeiro Login 
                case 58:
                {
                    x = entrarPortao();  
                    if (x == 1) {
                        lcd_cmd(L_CLR); // Clear LCD Display
                        lcd_cmd(L_L1); // Escreve na linha 1
                        lcd_str("Acesso Permitido!");
                        __delay_ms(2000); //Delay de 2 segundo para não sobrepor dados na memória
                        for (i = 0; i < 4; i++) { // Recebendo os quatros digitos da senha
                            __delay_ms(300);//Delay de 0.3 segundo para não sobrepor dados na memória
                            RA1 ^= 1;
                            __delay_ms(300); //Delay de 0.3 segundo para não sobrepor dados na memória
                            RA2 ^= 1;
                            __delay_ms(300); //Delay de 0.3 segundo para não sobrepor dados na memória
                            PORTA ^= 0x40; //RA6=1;
                            __delay_ms(300); //Delay de 0.3 segundo para não sobrepor dados na memória
                            PORTA ^= 0x80; //RA7=1;
                            __delay_ms(300); //Delay de 0.3 segundo para não sobrepor dados na memória
                        }
                        lcd_cmd(L_OFF); // Desliga o LCD 
                    } else {
                        lcd_cmd(L_CLR); // Clear LCD Display
                        lcd_cmd(L_L1); // Escreve na linha 1
                        lcd_str("Acesso Negado!");
                        __delay_ms(2000); //Delay de 2 segundo para não sobrepor dados na memória
                        lcd_cmd(L_OFF); // Desliga o LCD 
                    }
                }
                break;

                case 60:
                {
                    configurarPortao(); // Insenrir novo usuário 
                }
                break;

                default:
                {
                    lcd_cmd(L_CLR); // Clear LCD Display
                    lcd_cmd(L_L1); // Escreve na linha 1
                    lcd_str("Opcao Invalida!");
                    __delay_ms(2000); //Delay de 2 segundo para não sobrepor dados na memória
                    lcd_cmd(L_OFF); // Desliga o LCD 
                }
            }
        }
    }
}

