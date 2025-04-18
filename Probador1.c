                                                               #include <xc.h>
#include <stdbool.h>

// Configuración del microcontrolador (Fuses)
#pragma config FOSC = XT     // Oscilador XT (Cristal de 4 MHz)
#pragma config WDTE = OFF    // Watchdog Timer desactivado
#pragma config PWRTE = ON    // Power-up Timer activado
#pragma config BOREN = ON    // Brown-out Reset activado
#pragma config LVP = OFF     // Low Voltage Programming desactivado
#pragma config CPD = OFF     // Protección de datos desactivada
#pragma config WRT = OFF     // Protección de escritura desactivada
#pragma config CP = OFF      // Protección de código desactivada


#define _XTAL_FREQ 4000000UL
#define salida  0
#define entrada 1
#define pul_ok RE0
#define pul_no RE1
#define ledok RB0

//CASOS
#define puertoa 0
#define puertob 1
#define puertoc 2
#define puertod 3
#define puertoe 4

char puerto;
bool xx = 0;

void main(void) {
    puerto = puertoa;
    TRISA = 0b0000000;
    TRISB = 0b0000000;
    TRISC = 0b0000000;
    TRISD = 0b0000000;
    TRISE = 0b00000111; // Configurar RE0, RE1 y RE2 como entradas
    ADCON1 = 0b00000111;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;


    while (1) {
        //ESPERA A QUE SE OPRIMA EL BOTON OK PARA EMPEZAR
        while (pul_ok == 0) {
            if (xx == 1) {
                break;
            }
            continue;
        }
        xx = 1; // variable que indica que se comenzo a probar las salidas

        switch (puerto) {
            case puertoa: //prueba las salidas del puerto A
                for (char i = 0; i < 7; i++) {
                    PORTA = (1 << i);
                    __delay_ms(500);
                }
                while (1) {
                    if (pul_no == 1) {
                        puerto = puertoa;
                        break;
                    }
                    if (pul_ok == 1) {
                        puerto = puertob;
                        break;
                    }
                }
                break;

            case puertob: // prueba las salidas del puerto B             
                for (char i = 0; i < 9; i++) {
                    PORTB = (1 << i);
                    __delay_ms(500);
                }
                while (1) {
                    if (pul_no == 1) {
                        puerto = puertob;
                        break;
                    }
                    if (pul_ok == 1) {
                        puerto = puertoc;
                        break;
                    }
                }
                break;

            case puertoc: // prueba las salidas del puerto C             
                for (char i = 0; i < 9; i++) {
                    PORTC = (1 << i);
                    __delay_ms(500);
                }
                while (1) {
                    if (pul_no == 1) {
                        puerto = puertoc;
                        break;
                    }
                    if (pul_ok == 1) {
                        puerto = puertod;
                        break;
                    }
                }
                break;

            case puertod: // prueba las salidas del puerto C             
                for (char i = 0; i < 9; i++) {
                    PORTD = (1 << i);
                    __delay_ms(500);
                }
                while (1) {
                    if (pul_no == 1) {
                        puerto = puertod;
                        break;
                    }
                    if (pul_ok == 1) {
                        puerto = puertod;
                        break;
                    }
                }
                break;
        }
    }
    __delay_ms(500);
}





