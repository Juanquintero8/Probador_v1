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

#define tiempo_prueba 150

unsigned char puerto, mask, port_act, nro_pin = 7;
bool xx = 0, prueba = 1, caso = 0;

void port_conf_r1(void);
void port_conf_r2(void);
void porte_ent(void);
void porte_sal(void);
void Prueba_sal(void);
void salidas(unsigned char i);

void main(void) {
    port_conf_r1();
    porte_ent();
    port_act = PORTA;
    puerto = puertoa;

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
                if (prueba == 0) {
                    port_conf_r1();
                    porte_ent();
                    Prueba_sal();
                    while (1) {
                        if (pul_no == 1) {
                            puerto = puertoa;
                            break;
                        }
                        if (pul_ok == 1) {
                            puerto = puertob;
                            nro_pin = 9;
                            break;
                        }
                    }
                    break;
                } else { //////RUTINA PARA PROBAR LAS ENTRADAS/////
                    port_conf_r2();
                    for (unsigned char i = 0; i < nro_pin; i++) { 
                        mask = (1 << i);
                        while (1) {
                            if (PORTA & mask) { 
                                RE2 = 1;
                                __delay_ms(tiempo_prueba);
                                break;                          
                            } if ( i == 6){
                                puerto = puertob;
                                nro_pin = 9;
                                RE2 = 0;
                                break;
                            } 
                            else { 
                                RE2 = 0;
                            }
                        }
                    }
                    
                    break;
                }
            case puertob: // prueba las salidas del puerto B  
                caso =1 ;
                if (prueba == 0) {
                    port_conf_r1();
                    porte_ent();
                    Prueba_sal();
                    while (1) {
                        if (pul_no == 1) {
                            puerto = puertob;
                            break;
                        }
                        if (pul_ok == 1) {
                            puerto = puertoc;
                            break;
                        }
                        break;
                    }
                    break;
                } else {
                    port_conf_r2();
                    for (unsigned char i = 0; i < nro_pin; i++) { // Solo RA0 a RA5
                        mask = (1 << i);
                        while (1) {
                            if (PORTB & mask) { // Si el pin est� en alto
                                RE2 = 1;
                                __delay_ms(tiempo_prueba);
                                break;
                            } else { // Si el pin est� en alto
                                RE2 = 0;
                            }
                        }
                    }
                    puerto = puertob;
                    nro_pin = 9;
                    break;
                }


                    case puertoc: // prueba las salidas del puerto C             
                    Prueba_sal();
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
                    Prueba_sal();
                    while (1) {
                        if (pul_no == 1) {
                            puerto = puertod;
                            break;
                        }
                        if (pul_ok == 1) {
                            puerto = puertoe;
                            nro_pin = 4;
                            porte_sal();
                            break;
                        }
                    }
                    break;

                    case puertoe: // prueba las salidas del puerto C             
                    Prueba_sal();
                    porte_ent();
                    while (1) {
                        if (pul_no == 1) {
                            puerto = puertoe;
                            porte_sal();
                            break;
                        }
                        if (pul_ok == 1) {
                            puerto = puertoa;
                            nro_pin = 7;
                            break;
                        }
                    }
                    break;
                }
                __delay_ms(250);
                /*       } else {
            

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

                                }*/
        }
    }

    void port_conf_r2(void) {
        TRISA = 0xFF;
        TRISB = 0xFF;
        TRISC = 0xFF;
        TRISD = 0xFF;
        PORTE = 0x00;
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

    void Prueba_sal(void) {
        for (unsigned char i = 0; i < nro_pin; i++) {
            salidas(i);
        }
    }

    void salidas(unsigned char i) {
        switch (puerto) {
            case puertoa:
                PORTA = (1 << i);
                __delay_ms(tiempo_prueba);
                break;
            case puertob:
                PORTB = (1 << i);
                __delay_ms(tiempo_prueba);
                break;
            case puertoc:
                PORTC = (1 << i);
                __delay_ms(tiempo_prueba);
                break;
            case puertod:
                PORTD = (1 << i);
                __delay_ms(tiempo_prueba);
                break;
            case puertoe:
                PORTE = (1 << i);
                __delay_ms(tiempo_prueba);
                break;
        }
    }

