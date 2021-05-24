/* 
 * File:   Portao_Eletronico.c
 * Alunos:      Charles Assis Torres Silva 
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
//#include "memoria.h"
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
        teclaX = tc_tecla(3000) + 0x30;
        if (teclaX != 0) {
            lcd_init();
            lcd_cmd(L_CLR);
            lcd_cmd(L_L1);
            lcd_str("Pressione * para");
            lcd_cmd(L_L2);
            lcd_str("entrar ou #");
            __delay_ms(1500);
            lcd_cmd(L_CLR);
            lcd_cmd(L_L1);
            lcd_str("para configurar");
            

            teclaX = tc_tecla(3000) + 0x30;

            switch (teclaX) {
                case 58:
                {
                    x = entrarPortao();
                    if (x == 1) {
                        lcd_cmd(L_CLR);
                        lcd_cmd(L_L1);
                        lcd_str("Acesso Permitido!");
                        __delay_ms(2000);
                        for (i = 0; i < 4; i++) {
                            __delay_ms(300);
                            RA1 ^= 1;
                            __delay_ms(300);
                            RA2 ^= 1;
                            __delay_ms(300);
                            PORTA ^= 0x40; //RA6=1;
                            __delay_ms(300);
                            PORTA ^= 0x80; //RA7=1;
                            __delay_ms(300);
                        }
                        lcd_cmd(L_OFF);
                    } else {
                        lcd_cmd(L_CLR);
                        lcd_cmd(L_L1);
                        lcd_str("Acesso Negado!");
                        __delay_ms(2000);
                        lcd_cmd(L_OFF);
                    }
                }
                break;

                case 60:
                {
                    configurarPortao();
                }
                break;

                default:
                {
                    lcd_cmd(L_CLR);
                    lcd_cmd(L_L1);
                    lcd_str("Opcao Invalida!");
                    __delay_ms(2000);
                    lcd_cmd(L_OFF);
                }
            }
        }
    }
}

