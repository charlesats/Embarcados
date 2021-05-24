#include "funcoes_Portao.h"
#include "lcd.h"
#include "teclado.h"

unsigned char entrarPortao() {

    unsigned char i, x;
    unsigned char teclaX;
    unsigned char pin[4];
    unsigned char pin1[4];


    lcd_cmd(L_CLR);
    lcd_cmd(L_L1 + 2);
    lcd_str("Digite o PIN");
    lcd_cmd(L_L2 + 3);
    lcd_str("e aperte *");
    __delay_ms(1000);
    lcd_cmd(L_CLR);
    lcd_cmd(L_L2);

    lerSenha(&pin1);

    lerMemoria(&pin);


    teclaX = tc_tecla(3000) + 0x30;

    if (teclaX == 58) {
        x = testeSenha(pin, pin1);
        return x;
    } else
        return 0;
    /*if (x == 1) {
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
    }*/
}

void configurarPortao() {

    unsigned char x, op;
    unsigned char teclaX;
    unsigned char pin1[4];
    unsigned char pin2[4];
    
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Necessario");
    lcd_cmd(L_L2);
    lcd_str("Autenticacao!");
    __delay_ms(1000);
    
    if (entrarPortao()) {
        op = 0;
        while (op == 0) {
            lcd_cmd(L_CLR);
            lcd_cmd(L_L1 + 2);
            lcd_str("Digite o novo");
            lcd_cmd(L_L2 + 1);
            lcd_str("pin e aperte *");
            __delay_ms(2000);
            lcd_cmd(L_CLR);

            lerSenha(&pin1);

            teclaX = tc_tecla(3000) + 0x30;

            if (teclaX == 58) {
                lcd_cmd(L_CLR);
                lcd_cmd(L_L1);
                lcd_str("Digite novamente");
                lcd_cmd(L_L2);
                lcd_str("o pin e aperte *");
                __delay_ms(2000);
                lcd_cmd(L_CLR);
                lerSenha(&pin2);

                teclaX = tc_tecla(3000) + 0x30;

                if (teclaX == 58) {
                    x = testeSenha(pin1, pin2);
                    if (x == 1) {
                        escreverMemoria(&pin2);

                        lcd_cmd(L_CLR);
                        lcd_cmd(L_L1);
                        lcd_str("Usuario");
                        lcd_cmd(L_L2);
                        lcd_str("Cadastrado!");
                        __delay_ms(2000);
                        op = 1;
                        lcd_cmd(L_OFF);
                    } else {
                        lcd_cmd(L_CLR);
                        lcd_cmd(L_L1);
                        lcd_str("Senha nao");
                        lcd_cmd(L_L2);
                        lcd_str("Confere!");
                        __delay_ms(2000);
                    }
                }
            }
        }
    }
    else
    {
        lcd_cmd(L_CLR);
        lcd_cmd(L_L1);
        lcd_str("Usuario nao");
        lcd_cmd(L_L2);
        lcd_str("Autenticado!");
        __delay_ms(2000);
        lcd_cmd(L_OFF);
    }
        
        
    /*op = 0;
    while (op == 0) {
        lcd_cmd(L_CLR);
        lcd_cmd(L_L1 + 2);
        lcd_str("Digite o novo");
        lcd_cmd(L_L2 + 1);
        lcd_str("pin e aperte *");
        __delay_ms(2000);
        lcd_cmd(L_CLR);

        lerSenha(&pin1);

        teclaX = tc_tecla(3000) + 0x30;

        if (teclaX == 58) {
            lcd_cmd(L_CLR);
            lcd_cmd(L_L1);
            lcd_str("Digite novamente");
            lcd_cmd(L_L2);
            lcd_str("o pin e aperte *");
            __delay_ms(2000);
            lcd_cmd(L_CLR);
            lerSenha(&pin2);

            teclaX = tc_tecla(3000) + 0x30;

            if (teclaX == 58) {
                x = testeSenha(pin1, pin2);
                if (x == 1) {
                    escreverMemoria(&pin2);
                    
                    lcd_cmd(L_CLR);
                    lcd_cmd(L_L1);
                    lcd_str("Usuario");
                    lcd_cmd(L_L1);
                    lcd_str("Cadastrado!");
                    __delay_ms(2000);
                    op = 1;
                    lcd_cmd(L_OFF);
                } else {
                    lcd_cmd(L_CLR);
                    lcd_cmd(L_L1);
                    lcd_str("Senha nao");
                    lcd_cmd(L_L2);
                    lcd_str("Confere!");
                    __delay_ms(2000);
                }
            }
        }
    }*/
}

unsigned char testeSenha(unsigned char *pin1[4], unsigned char *pin2[4]) {

    unsigned char i, x;

    x = 1;
    i = 0;
    while (i < 4) {
        if (pin1[i] != pin2[i])
            x = 0;
        i++;
    }
    return x;
}

void lerSenha(unsigned char *pin1) {

    unsigned char i;
    unsigned char teclaX;

    i = 0;
    while (i < 4) {
        teclaX = tc_tecla(3000) + 0x30;
        if (teclaX != 0) {
            pin1[i] = teclaX;
            //lcd_cmd(L_CLR);
            lcd_cmd(L_L1);
            lcd_str("PIN:");
            lcd_cmd(L_L2 + i);
            lcd_str("*");
        }
        __delay_ms(200);
        i++;
    }
}

void iniciarMemoria() {
    unsigned char pin[4] = "1234"; // Senha padrão para testes
    escreverMemoria(&pin);
}

void lerMemoria(unsigned char *pin) {
    unsigned char i, j;
    char pos = 120;
    for (i = pos, j = 0; j < 4; i++, j++) {
        pin[j] = eeprom_read(i);
    }
}

void escreverMemoria(unsigned char *pin[4]) {
    unsigned char i, j, pos;
    pos = 120;

    for (i = pos, j = 0; j < 4; i++, j++) {
        eeprom_write(i, pin[j]);
        __delay_ms(10);
    }
}

