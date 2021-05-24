/* ########################################################################
   PICsim - PIC simulator http://sourceforge.net/projects/picsim/
   ########################################################################
   Copyright (c) : 2015  Luis Claudio Gambôa Lopes
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

#include <xc.h>
#include"serial.h"
#define _XTAL_FREQ 4000000 // 4MHz

void serial_init(void) {

    // BRGH = 1 (High Speed)
    // Baud Rate = FOSC/(16 (SPBRG + 1)) - Tempo de envio/recebimento de cada bit
    // SPBRG: Baud Rate Generator Register
    SPBRG = 25; // Baud Rate = 9600, Error (%) = 0.16

    TXSTAbits.TX9 = 0; // 0 = Transmissao de 8-bit
    TXSTAbits.TXEN = 1; // 1 = Transmissor habilitado
    TXSTAbits.SYNC = 0; // 0 = Modo assincrono
    TXSTAbits.BRGH = 1; // 1 = High speed

    RCSTAbits.SPEN = 1; // 1 = Porta serial habilitada (configures RC7/RX/DT and RC6/TX/CK pins as serial port pins)
    RCSTAbits.RX9 = 0;  // 0 = Recepção de 8-bit
    RCSTAbits.CREN = 1; // 1 = Receptor Contínuo habilitado



}

void serial_tx(unsigned char val) {   //Escreve no terminal serial
    TXREG = val;                      
    while (!TXSTAbits.TRMT);
}

void serial_tx_str(const char* val) { //Escreve strings no terminal serial  
    unsigned char i = 0;

    while (val[i]) {
        serial_tx(val[i]);
        i++;
    }
}

unsigned char serial_rx(unsigned int timeout) { //Recebe os valores da porta serial
    unsigned int to = 0;

    while (1) {
        if (RCSTAbits.FERR || RCSTAbits.OERR)   // FERR: Framing Error bit, OERR: Overrun Error bit
        {
            RCSTAbits.CREN = 0; // 0 = Disables continuous receive
            RCSTAbits.CREN = 1; // 1 = Enables continuous receive
        }
        
        if (PIR1bits.RCIF) //RCIF bit 5, leitura terminal serial
        {
            return RCREG;  //Retorna o que foi digitado
            break;
        }
    }
}