/*
 * ENGG53 - LABORATÓRIO INTEGRADO II
 * Integrantes: David Oliveira, David Ferrari, André Lino e Caio.
 * File:   main.c
 *
 */

#include <xc.h>
#include <p24FJ128GA110.h>

// Configurações do microcontrolador
#pragma config FNOSC = FRC      // Oscilador de 8 MHz
#pragma config FCKSM = CSDCMD   // Desativar comutação de clock e monitor de fail-safe
#pragma config FWDTEN = OFF     // Desativar Watchdog Timer

// Definindo os LEDs nos pinos RA0, RA1 e RA2.
#define BUZZER LATAbits.LATA0
#define LED1   LATAbits.LATA1
#define LED2   LATAbits.LATA2
#define LED3   LATAbits.LATA3

// Definindo os sensores e o alarme nos pinos RF0, RF1 e RF2.
#define BOTAO_ALARME PORTFbits.RF0
#define SENSOR1      PORTFbits.RF1
#define SENSOR2      PORTFbits.RF2

void configurarPinos(void) {
    TRISF = 0xFFFF; // Configura todos os pinos RF como entrada.
    TRISA = 0x0000; // Configura todos os pinos RA como saída.
    //TRISBbits.TRISB0 = 1; // Configura o pino RB0 (AN0) como entrada. 
}

void inicializarADC(void) {
    AD1CON1bits.ADON = 0;  // Desliga o ADC
    AD1CON1bits.FORM = 0;  // Saída em inteiro
    AD1CON1bits.SSRC = 7;  // Modo de amostragem automática
    AD1CON2bits.VCFG = 0;  // Referências VDD e VSS
    AD1CON3bits.ADCS = 2;  // Clock do ADC
    AD1CON3bits.SAMC = 2;  // Tempo de amostragem

    AD1CHS = 0;            // Inicializa o canal de amostragem para AN0
//    AD1CON2bits.CSCNA = 1; // Habilita a varredura de canais;
//    AD1CSSL = 0x0003;      // Seleciona AN0 e AN1 para varredura
    AD1CON1bits.ADON = 1;  // Liga o ADC
}

float lerADC(unsigned int canal, float valorMaximo) {
    AD1CHS = canal;             // Seleciona o canal de amostragem
    AD1CON1bits.SAMP = 1;       // Inicia a amostragem
    while (!AD1CON1bits.DONE);  // Espera a conversão completar
    float valorADCNormalizado = (ADC1BUF0 * valorMaximo) / 1023; //Normaliza o valor considerando inteiros. (0 até 1023)
    return valorADCNormalizado; // Retorna o valor convertido
}


void inicializarSaidas(void) {
    // Estado inicial dos LEDs e Buzzer (desligados)
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    BUZZER = 0;
}

void verificarDesligamentoBuzzer(void) {
    if (LED1 == 0 && LED2 == 0 && LED3 == 0) { // Desliga o buzzer se nenhum LED estiver ligado.
        BUZZER = 0;
    }
}

//int resetarAlertaConfirmado(void) {;
//    int _alertaConfirmado;
////    if (LED1 == 0 && LED2 == 0 && LED3 == 0) {
//        return _alertaConfirmado = 0; // Reseta indicando que nenhum alerta foi confirmado ainda.
////    }
//}

int main(void) {
    
    configurarPinos();
    inicializarADC();
    inicializarSaidas();
    
    float voltagemLimite = 3.5;
    int alertaConfirmado = 0;
    
    while (1) {
        
        // Verifica se o SENSOR1 foi pressionado para ligar/desligar o LED1
        if (SENSOR1 == 1) {
            while (SENSOR1 == 1); // Aguarda botão ser solto.
            LED1 = !LED1; // Alterna o estado do LED1.
            if (LED1 == 1) {
                BUZZER = 1; // Liga o buzzer se o LED1 foi ligado.
            }
            verificarDesligamentoBuzzer();
        }
        
        // Da mesma forma o SENSOR2.
        if (SENSOR2 == 1) {
            while (SENSOR2 == 1);
            LED2 = !LED2;
            if (LED2 == 1) {
                BUZZER = 1;
            }
            verificarDesligamentoBuzzer();
        }
        
        // Verifica se BOTAO_ALARME foi pressionado.
        if (BOTAO_ALARME == 1) {
            while (BOTAO_ALARME == 1);
            BUZZER = 0;
            alertaConfirmado = 1; // Confirma o alerta atual. Util para o SENSOR3.
        }
        
        // Verifica constantemente o SENSOR3(Potêncimetro) e atualiza o LED3.
        unsigned int sensor3 = lerADC(0, 5.0); // Lê o valor normalizado na entrada analogica AN0/RB0 no pic24.
        if (sensor3 > voltagemLimite) {
            LED3 = 1;
            if (!alertaConfirmado){ // O BUZZER só é ativado se não tiver alertas confirmados.
                BUZZER = 1;
            }
        } else {
            LED3 = 0;
            alertaConfirmado = 0;
            verificarDesligamentoBuzzer();
        }
    }

    return 0;
}
