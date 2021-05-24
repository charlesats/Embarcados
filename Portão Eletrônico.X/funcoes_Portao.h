/* 
 * File:   Portao_Eletronico.c
 * Alunos:      Charles Assis Torres Silva 
 *              
 * 
 * Professor:   Marcelo Moreira Tiago 
 *
 * Created on 7 de Abril de 2021, 10:19
 */

#include <xc.h>
#include <string.h>
#include "lcd.h"
#include "teclado.h"
//#include "memoria.h"
#define _XTAL_FREQ 4000000 // 4MHz


unsigned char testeSenha(unsigned char *pin1[4], unsigned char *pin2[4]);
void lerSenha(unsigned char *pin1);
unsigned char entrarPortao();
void configurarPortao();

void iniciarMemoria();
void lerMemoria(unsigned char *pin);
void escreverMemoria(unsigned char *pin[4]);
    