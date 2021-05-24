#include "funcoes_Portao.h"
#include "lcd.h"
#include "teclado.h"

unsigned char entrarPortao() {
    // Declara��o de variaveis 
    unsigned char i, x; 
    unsigned char teclaX;
    unsigned char pin[4]; // Vetor 
    unsigned char pin1[4]; // Vetor 


    lcd_cmd(L_CLR); // Clear LCD Display
    lcd_cmd(L_L1 + 2); // Escreve na linha 1
    lcd_str("Digite o PIN");
    lcd_cmd(L_L2 + 3); // Escreve na linha 2
    lcd_str("e aperte *");
    __delay_ms(1000);
    lcd_cmd(L_CLR); // Clear LCD Display
    lcd_cmd(L_L2); // Escreve na linha 2

    lerSenha(&pin1); // Ler pin digitado 

    lerMemoria(&pin); // Ler Pin de fabrica 


    teclaX = tc_tecla(3000) + 0x30; // Soma para pegar o n�meros corretos pela tabela ASCII

    if (teclaX == 58) {
        x = testeSenha(pin, pin1); // Compara as duas senhas 
        return x;
    } else
        return 0;
   
}

void configurarPortao() {

    unsigned char x, op;
    unsigned char teclaX;
    unsigned char pin1[4];
    unsigned char pin2[4];
    
    lcd_cmd(L_CLR); // Clear LCD Display
    lcd_cmd(L_L1); // Escreve na linha 1
    lcd_str("Necessario");
    lcd_cmd(L_L2); // Escreve na linha 2
    lcd_str("Autenticacao!");
    __delay_ms(1000); //Delay de 1 segundo para n�o sobrepor dados na mem�ria
    
    if (entrarPortao()) { // Cadastra novo usu�rio
        op = 0;
        while (op == 0) {
            lcd_cmd(L_CLR); // Clear LCD Display
            lcd_cmd(L_L1 + 2); // Escreve na linha 1
            lcd_str("Digite o novo"); 
            lcd_cmd(L_L2 + 1); // Escreve na linha 2
            lcd_str("pin e aperte *");
            __delay_ms(2000); //Delay de 2 segundo para n�o sobrepor dados na mem�ria
            lcd_cmd(L_CLR); // Clear LCD Display

            lerSenha(&pin1);  
 
            teclaX = tc_tecla(3000) + 0x30;  // Soma para pegar o n�meros corretos pela tabela ASCII

            if (teclaX == 58) {
                lcd_cmd(L_CLR); // Clear LCD Display
                lcd_cmd(L_L1); // Escreve na linha 1
                lcd_str("Digite novamente");
                lcd_cmd(L_L2); // Escreve na linha 2
                lcd_str("o pin e aperte *");
                __delay_ms(2000); //Delay de 2 segundo para n�o sobrepor dados na mem�ria
                lcd_cmd(L_CLR); // Clear LCD Display
                lerSenha(&pin2); // Salva na mem�ria a segunda senha digitada  

                teclaX = tc_tecla(3000) + 0x30; // Soma para pegar o n�meros corretos pela tabela ASCII

                if (teclaX == 58) {
                    x = testeSenha(pin1, pin2); // Comparada as duas senhas 
                    if (x == 1) {
                        escreverMemoria(&pin2); // Cadastro de usu�rio 

                        lcd_cmd(L_CLR); // Clear LCD Display
                        lcd_cmd(L_L1); // Escreve na linha 1
                        lcd_str("Usuario");
                        lcd_cmd(L_L2); // Escreve na linha 2
                        lcd_str("Cadastrado!");
                        __delay_ms(2000); //Delay de 2 segundo para n�o sobrepor dados na mem�ria
                        op = 1;
                        lcd_cmd(L_OFF); // Desliga o LCD 
                    } else { // Caso a senha n�o batam 
                        lcd_cmd(L_CLR); // Clear LCD Display
                        lcd_cmd(L_L1); // Escreve na linha 1
                        lcd_str("Senha nao");
                        lcd_cmd(L_L2); // Escreve na linha 2
                        lcd_str("Confere!");
                        __delay_ms(2000); //Delay de 2  segundo para n�o sobrepor dados na mem�ria
                    }
                }
            }
        }
    }
    else // Caso erre a senha de fabrica 
    {
        lcd_cmd(L_CLR); // Clear LCD Display
        lcd_cmd(L_L1);// Escreve na linha 1
        lcd_str("Usuario nao");
        lcd_cmd(L_L2);// Escreve na linha 2
        lcd_str("Autenticado!");
        __delay_ms(2000); //Delay de 2 segundo para n�o sobrepor dados na mem�ria
        lcd_cmd(L_OFF); // Desliga o LCD 
    }
        
        
    
}

unsigned char testeSenha(unsigned char *pin1[4], unsigned char *pin2[4]) { //Realiza a compara��o das senhas percorrendo o vetor
                                                                           //comparando vari�vel por vari�vel
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
        teclaX = tc_tecla(3000) + 0x30; // Soma para pegar o n�meros corretos pela tabela ASCII
        if (teclaX != 0) {
            pin1[i] = teclaX; // Salva o numero digitado na posi��o 'i'
            //lcd_cmd(L_CLR);
            lcd_cmd(L_L1); // Escreve na linha 1
            lcd_str("PIN:");
            lcd_cmd(L_L2 + i); // Escreve na linha 2
            lcd_str("*");
        }
        __delay_ms(200); //Delay de 0.2 segundo para n�o sobrepor dados na mem�ria
        i++;
    }
}
 // Senha Padr�o para inciar 
void iniciarMemoria() {
    unsigned char pin[4] = "1234"; // Senha padr�o para testes
    escreverMemoria(&pin);
}
 // Chamando a fun��o lerMemoria
void lerMemoria(unsigned char *pin) {  //realiza a leitura da senha armazenada em um vetor de posi��o inicial 120 na EEPROM
    unsigned char i, j;
    char pos = 120;
    for (i = pos, j = 0; j < 4; i++, j++) {
        pin[j] = eeprom_read(i);
    }
}
 // Chamando a fun��o EscreverMemoria
void escreverMemoria(unsigned char *pin[4]) { //Salva a senha na EEPROM no formato de um vetor 
    unsigned char i, j, pos;                  //do tipo char com quatro vari�veis
    pos = 120;

    for (i = pos, j = 0; j < 4; i++, j++) {
        eeprom_write(i, pin[j]); 
        __delay_ms(10);
    }
}

