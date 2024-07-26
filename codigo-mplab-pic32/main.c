/*
 * ENGG53 - LABORATÓRIO INTEGRADO II
 * Integrantes: David Oliveira, David Ferrari, André Lino e Caio.
 * File:   main.c
 *
 */


NOME LCD: TFT_TM022HDH26 ILI9341

#include <xc.h>

// Configurações do microcontrolador
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8


// Definindo os LEDs nos pinos RA0, RA1 e RA2.
#define BUZZER LATAbits.LATA0
#define LED1   LATAbits.LATA1
#define LED2   LATAbits.LATA2
#define LED3   LATAbits.LATA3

// Definindo os sensores e o alarme nos pinos RF0, RF1 e RF2.
#define BOTAO_ALARME PORTBbits.RB0
#define SENSOR1      PORTBbits.RB1
#define SENSOR2      PORTBbits.RB2

#define SHORT_DELAY (50*8)
#define LONG_DELAY	(4000*8)

main(void) {
    int botaoPressionado3 = 0;
    int botaoPressionado2 = 0;
    int botaoPressionado1 = 0;
    DDPCONbits.JTAGEN = 0;
    TRISA = 11110000;    TRISB = 1;
    T1CON = 0x8030; // TMR1 on, prescale 1:256 PB
    LATAbits.LATA0 = 0;
    LATAbits.LATA1 = 0;
    LATAbits.LATA2 = 0;
    LATAbits.LATA3 = 0;
                                                                               
    while(1) {
      
        
        if(PORTAbits.RA7 == 0 && botaoPressionado3 == 0) {
            
            LATAbits.LATA2 = 1;
            BUZZER = 1;
            TMR1 = 0;
            botaoPressionado3 = 1;
            while ( TMR1 < LONG_DELAY)
            {
            }
        }
        
        if(PORTAbits.RA7 == 0 && botaoPressionado3 == 1){
            LATAbits.LATA2 = 0;
            TMR1 = 0;
            BUZZER = 0;
            botaoPressionado3 = 0;
            while ( TMR1 < LONG_DELAY)
            {
            }
        }
      if(PORTAbits.RA6 == 0 && botaoPressionado2 == 0) {
            
            LATAbits.LATA1 = 1;
            BUZZER = 1;
            TMR1 = 0;
            botaoPressionado2 = 1;
            while ( TMR1 < LONG_DELAY)
            {
            }
        }
        
        if(PORTAbits.RA6 == 0 && botaoPressionado2 == 1){
            LATAbits.LATA1 = 0;
            TMR1 = 0;
            BUZZER = 0;
            botaoPressionado2 = 0;
            while ( TMR1 < LONG_DELAY)
            {
            }
        }
        if(PORTAbits.RA5 == 0) {
            
            BUZZER = 0;
//            TMR1 = 0;
//            while ( TMR1 < LONG_DELAY)
//            {
//            }
        }
    }
    
}