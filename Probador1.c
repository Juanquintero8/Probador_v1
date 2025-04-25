#include <xc.h>
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

#define tiempo_prueba 300
#define tiempo_prueba1 150

unsigned char puerto = puertoa, mask, port_act, nro_pin = 7, incremento = 0;
bool xx = 0, prueba = 0, caso = 0;

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
                    mask = 1;
                    incremento = 1;
                    for (unsigned char i = 0; i < nro_pin; i = incremento) {
                        while (1) {
                            if (PORTA & mask) {
                                RE2 = 1;
                                __delay_ms(tiempo_prueba);
                                RE2 = 0;
                                __delay_ms(50);
                                mask = (1 << incremento);
                                incremento++;
                            }
                            if (incremento == 7) {
                                puerto = puertob;
                                break;
                            }
                        }
                        break;
                    }
                }


            case puertob: // prueba las salidas del puerto B  
                if (prueba == 0) {
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
                    }
                    break;
                } else { //////RUTINA PARA PROBAR LAS ENTRADAS/////
                    incremento = 1;
                    nro_pin = 9;
                    mask = 1;
                    for (unsigned char i = 0; i < nro_pin; i = incremento) {
                        while (1) {
                            if (PORTB & mask) {
                                RE2 = 1;
                                __delay_ms(tiempo_prueba);
                                RE2 = 0;
                                __delay_ms(50);
                                mask = (1 << incremento);
                                incremento++;
                            }
                            if (incremento == 9) {
                                puerto = puertoc;
                                break;
                            }
                        }

                    }
                    break;
                }


            case puertoc: // prueba las salidas del puerto C             
                if (prueba == 0) {
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
                } else { //////RUTINA PARA PROBAR LAS ENTRADAS/////
                    incremento = 1;
                    nro_pin = 9;
                    mask = 1;
                    for (unsigned char i = 0; i < nro_pin; i = incremento) {
                        while (1) {
                            if (PORTC & mask) {
                                RE2 = 1;
                                __delay_ms(tiempo_prueba);
                                RE2 = 0;
                                __delay_ms(50);
                                mask = (1 << incremento);
                                incremento++;
                            }
                            if (incremento == 9) {
                                puerto = puertod;
                                break;
                            }
                        }

                    }
                    break;
                }

            case puertod: // prueba las salidas del puerto C             
                if (prueba == 0) {
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
                } else { //////RUTINA PARA PROBAR LAS ENTRADAS/////
                    incremento = 1;
                    nro_pin = 9;
                    mask = 1;
                    for (unsigned char i = 0; i < nro_pin; i = incremento) {
                        while (1) {
                            if (PORTD & mask) {
                                RE2 = 1;
                                __delay_ms(tiempo_prueba);
                                RE2 = 0;
                                __delay_ms(50);
                                mask = (1 << incremento);
                                incremento++;
                            }
                            if (incremento == 9) {
                                puerto = puertoe;
                                break;
                            }
                        }

                    }
                    break;
                }

            case puertoe: // prueba las salidas del puerto C             
                if (prueba == 0) {
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
                            prueba = 1;
                            nro_pin = 7;
                            break;
                        }
                    }
                    break;
                } else { //////RUTINA PARA PROBAR LAS ENTRADAS/////
                    incremento = 1;
                    nro_pin = 3;
                    mask = 1;
                    for (unsigned char i = 0; i < nro_pin; i = incremento) {
                        while (1) {
                            if (PORTE & mask) {
                                RE2 = 1;
                                __delay_ms(tiempo_prueba);
                                RE2 = 0;
                                __delay_ms(50);
                                mask = (1 << incremento);
                                incremento++;
                            }
                            if (incremento == 4) {
                                puerto = puertoa;
                                nro_pin = 7;
                                prueba = 0;
                                break;
                            }
                        }
                        break;
                    }

                }
        }
    }
    __delay_ms(250);
    /*       } else {
            

                for (unsigned char i = 0; i < 6; i++) { // Solo RA0 a RA5
                    mask = (1 << i); // Máscara para verificar un solo bit

                    if (PORTA & mask) { // Si el pin está en alto
                        RD0 = 1;
                        __delay_ms(tiempo_prueba);
                    } else {
                        RD0 = 0; // Apaga RD0
                    }
                    __delay_ms(150);
                }

                    }*/
}

void port_conf_r2(void) {
    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xFF;
    TRISD = 0xFF;
    PORTE = 0x00;
}

void port_conf_r1(void) {
    //puerto = puertoa;
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
            __delay_ms(tiempo_prueba1);
            break;
        case puertob:
            PORTB = (1 << i);
            __delay_ms(tiempo_prueba1);
            break;
        case puertoc:
            PORTC = (1 << i);
            __delay_ms(tiempo_prueba1);
            break;
        case puertod:
            PORTD = (1 << i);
            __delay_ms(tiempo_prueba1);
            break;
        case puertoe:
            PORTE = (1 << i);
            __delay_ms(tiempo_prueba1);
            break;
    }
}

// mask 00000001
//porta 00000001
