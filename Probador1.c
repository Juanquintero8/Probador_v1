#include <xc.h>
#include <stdbool.h>
#include <stdio.h>

// Configuración del microcontrolador (Fuses)
#pragma config FOSC = HS        // Oscilador de alta velocidad (para 8 MHz)
#pragma config WDTE = OFF       // Desactiva Watchdog Timer
#pragma config PWRTE = OFF      // Desactiva Power-up Timer
#pragma config BOREN = ON       // Activa reinicio por caída de voltaje
#pragma config LVP = OFF        // Desactiva programación por baja tensión
#pragma config CPD = OFF        // No protege la EEPROM
#pragma config WRT = OFF        // No protege la memoria de programa
#pragma config CP = OFF         // No protege el código


#define _XTAL_FREQ 8000000UL
#define salida  0
#define entrada 1
#define pul_ok RE0
#define pul_no RE1
#define ledok RB0
#define MIN 3

//CASOS
#define puertoa 0
#define puertob 1
#define puertoc 2
#define puertod 3
#define puertoe 4

#define tiempo_prueba 300
#define tiempo_prueba1 150

unsigned char puerto = puertoa, mask, port_act, nro_pin = 7, incremento = 0, prueba = 0;
bool xx = 0, caso = 0;

void port_conf_r1(void);
void port_conf_r2(void);
void porte_ent(void);
void porte_sal(void);
void Prueba_sal(void);
void salidas(unsigned char i);


// ===== UART =====
void UART_Init(void) {
    TXSTAbits.BRGH = 1;    // Alta velocidad
    SPBRG = 25;            // 9600 baudios para 4 MHz y BRGH=1
    RCSTAbits.SPEN = 1;    // Habilita el puerto serie (TX/ RX)
    TXSTAbits.TXEN = 1;    // Habilita transmisión
    RCSTAbits.CREN = 1;    // Habilita recepción
}

void UART_Write(char data) {
    while (!TXSTAbits.TRMT);  // Espera que el buffer esté vacío
    TXREG = data;
}

void putch(char data) {
    UART_Write(data);
}

// ===== ADC =====
void ADC_Init(void) {
    ADCON1 = 0b11000000;           // AN analógico, resto digital
    ADCON0 = 0b00001001;       
    __delay_ms(10);            // Tiempo para estabilizar
}

unsigned int ADC_Read(unsigned char channel) {
    ADCON0 &= 0x11000101; //Clearing the Channel Selection Bits
    ADCON0 |= channel <<3; //Setting the required Bits
    __delay_ms(2);           // Tiempo de adquisición
    GO_nDONE = 1;            // Inicia conversión
    while(GO_nDONE);         // Espera a que termine
    return ((ADRESH << 8) + ADRESL);  // Devuelve resultado 10 bits
}

// ===== MAIN =====
void ADC(void) {
    UART_Init();
    ADC_Init();
    TRISB = 0b00000000; // todo el puerto B con salidas
    //TRISA0 = 1; 
    
    printf("inicio");

    for(unsigned int i; i <(MIN*60/0.150);i++) { /// REPITE ESTE BUCLE POR 3 MINUTOS
        
        unsigned int valor = ADC_Read(1);
        printf("AN7 = %u\r\n", valor);
        if (valor >= 0 & valor < 125 ){
            PORTB = 0b010000000;
        }
        if (valor >= 125 & valor < 250 ){
            PORTB = 0b11000000;
        }
        if (valor >= 250 & valor < 375 ){
            PORTB = 0b11100000;
        }
        if (valor >= 375 & valor < 500 ){
            PORTB = 0b11110000;
        }
        if (valor >= 500 & valor < 625 ){
            PORTB = 0b11111000;
        }
        if (valor >= 625 & valor < 750 ){
            PORTB = 0b11111100;
        }
        if (valor >= 750 & valor < 875 ){
            PORTB = 0b11111110;
        }
        if (valor >= 875 & valor < 1025 ){
            PORTB = 0b11111111;
        }
        
        __delay_ms(150);
    }
}


void main(void) { //BUCLE PRINCIPAL
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
                break;



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
                                prueba = 2;
                                break;
                            }
                        }
                        break;
                    }

                }
        }
        
        if (prueba == 2){
            ADC();
            prueba = 3;
        }
        
        if (prueba == 3) { // RUTINA FINAL          
            port_conf_r1();
            porte_sal();
            for (unsigned char i; i < 3; i++) {
                PORTE = 0xFF;
                __delay_ms(1000);
                PORTE = 0;
                __delay_ms(1000);
                
            }
            prueba = 0;
            xx = 0;
        }
    }
    __delay_ms(250);
}

void port_conf_r2(void) {
    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xFF;
    TRISD = 0xFF;
    PORTE = 0x00;
}

void port_conf_r1(void) {
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
