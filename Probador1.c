                                                               #include <xc.h>

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
#define led1 RA0
#define salida 0
#define entrada 0

void blink (void);

void main(void){
    TRISA = 0b0000000;  
    while(1){
        for (char i=0;i<7;i++){
            PORTA = (1 << i);
            __delay_ms(500);
            //blink();
        }
    }
}

void blink (void){
        led1 = 1;
        __delay_ms(500);
        led1 = 0;
        __delay_ms(500);
}