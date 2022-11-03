#define F_CPU 3200000UL
#define dit 200
#define delay 400
#include <avr/io.h>
#include <util/delay.h>

void dot (void){
    PORTA_OUT = 0b00000000; 
    _delay_ms(dit);
    PORTA_OUT = 0b11111111;
    _delay_ms(dit);
}

void dash (void){
    PORTA_OUT = 0b00000000;
    _delay_ms(dit*3);
    PORTA_OUT = 0b11111111;
    _delay_ms(dit);
}

int main(void)
{
    PORTA_DIR = 0b11111111;
    PORTA_OUT = 0b11111111;
    PORTC_DIR = 0b00100001;
    PORTC_OUT = 0b11111110;
    while(1){
        for(i = 0; i < 3; i++))){ dot(); } // The letter S
        _delay_ms(delay);
        for(i = 0; i < 3; i++))){ dash(); } // The letter O
        _delay_ms(delay);
        for(i = 0; i < 3; i++))){{ dot(); } // The letter S
        _delay_ms((delay*3)+dit);
    }
    return(0);
}

