                                                               #include <xc.h>
#include <stdbool.h>

// Configuraci�n del microcontrolador (Fuses)
#pragma config FOSC = XT     // Oscilador XT (Cristal de 4 MHz)
#pragma config WDTE = OFF    // Watchdog Timer desactivado
#pragma config PWRTE = ON    // Power-up Timer activado
#pragma config BOREN = ON    // Brown-out Reset activado
#pragma config LVP = OFF     // Low Voltage Programming desactivado
#pragma config CPD = OFF     // Protecci�n de datos desactivada
#pragma config WRT = OFF     // Protecci�n de escritura desactivada
#pragma config CP = OFF      // Protecci�n de c�digo desactivada


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

#define tiempo_prueba 500

unsigned char puerto, mask;
bool xx = 0, prueba = 1;

void port_conf_r1(void);
void porte_ent(void);
void porte_sal(void);

void main(void) {
    port_conf_r1();
    porte_ent();

    while (1) {
        //ESPERA A QUE SE OPRIMA EL BOTON OK PARA EMPEZAR
        while (pul_ok == 0) {
            if (xx == 1) {
                break;
            }
            continue;
        }
        xx = 1; // variable que indica que se comenzo a probar las salidas
        if (prueba == 0) {
            switch (puerto) {
                case puertoa: //prueba las salidas del puerto A
                    for (unsigned char i = 0; i < 7; i++) {
                        PORTA = (1 << i);
                        __delay_ms(tiempo_prueba);
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
                    for (unsigned char i = 0; i < 9; i++) {
                        PORTB = (1 << i);
                        __delay_ms(tiempo_prueba);
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
                    for (unsigned char i = 0; i < 9; i++) {
                        PORTC = (1 << i);
                        __delay_ms(tiempo_prueba);
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
                    for (unsigned char i = 0; i < 9; i++) {
                        PORTD = (1 << i);
                        __delay_ms(tiempo_prueba);
                    }
                    while (1) {
                        if (pul_no == 1) {
                            puerto = puertod;
                            break;
                        }
                        if (pul_ok == 1) {
                            puerto = puertoe;
                            porte_sal();
                            break;
                        }
                    }
                    break;

                case puertoe: // prueba las salidas del puerto C             
                    for (unsigned char i = 0; i < 4; i++) {
                        PORTE = (1 << i);
                        __delay_ms(tiempo_prueba);
                    }
                    porte_ent();
                    while (1) {
                        if (pul_no == 1) {
                            puerto = puertoe;
                            porte_sal();
                            break;
                        }
                        if (pul_ok == 1) {
                            puerto = puertoa;
                            break;
                        }
                    }
                    break;
            }
            __delay_ms(500);
        } else {
            TRISA = 0b11111111; // Todo PORTA como entradas
            PORTD = 0x00;

            for (unsigned char i = 0; i < 6; i++) { // Solo RA0 a RA5
                mask = (1 << i); // M�scara para verificar un solo bit

                if (PORTA & mask) { // Si el pin est� en alto
                    RD0 = 1;
                    __delay_ms(tiempo_prueba);
                } else {
                    RD0 = 0; // Apaga RD0
                }
                __delay_ms(150);
            }

        }
    }
}
void port_conf_r1(void) {
    puerto = puertoa;
    TRISA = 0b00000000;
    TRISB = 0b00000000;
    TRISC = 0b00000000;
    TRISD = 0b00000000;
    ADCON1 = 0b00000111;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
}

void porte_ent(void) {
    TRISE = 0b00000011;
}

void porte_sal(void) {
    TRISE = 0b00000000; // Configurar RE0, RE1 y RE2 como entradas
}




