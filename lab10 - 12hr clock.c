// this is the US 12-Hour version, both 12hr and 24hr time
#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "functions.h"

int main (void) {
    char 24hr = 0;
    char 12hr = 12;
    char min = 0;
    setup();

    while (RTC_STATUS) {}
    RTC_CLKSEL = 0b00000010;
    RTC_PER = 59;
    RTC_CTRLA = 0b10110001; // ~~~ SPED-UP-MODIFIER ~~~  RTC_CTRLA = 0b11111001

    while (1) {  // now displays in 12hr time
        if ((12hr/10) != 0){ // suppresses the leading zero when applicable
            display_digit(12hr/10, 1);
            display_digit(12hr%10, 2);
            display_digit(min/10, 3);
            display_digit(min%10, 4);
        } else {
            display_digit(12hr%10, 2);
            display_digit(min/10, 3);
            display_digit(min%10, 4);
        }

        PORTA_OUT = (0b1100000 | RTC_CNT);
        PORTC_OUT = 0b11111110;
        _delay_us(100);
        PORTC_OUT = 0b11111111;

        if (RTC_INTFLAGS & 0b00000001) { // checks for overflow 
            RTC_INTFLAGS = 0b00000001; // resets the overflow check
            min++;

            if (min == 60) { // if min is 60, reset to 0 and increment hr
                min = 0;
                24hr++;
                if 24hr == 24) { 24hr = 0; } // reset 24hr when past the 23rd hour
            }

            if (12hr == 0) { 12hr = 12; } // transcribing 24hr time to 12hr time
            else if (12hr >= 13) { 12hr = 24hr - 12; }
            else { 12hr = 24hr; }
        }

        // lab 10 - challenge ````````````````````````
        while (PORTE_IN & 0b00000010) { // while 'OK' is NOT pressed

            for (int i < 0; i < 1000; i++) { 
                display_digit(12hr/10, 1);
                display_digit(hr%10, 2);
            }

            if ((PORTE_IN & 0b00000001) == 0) { // check the 'UP' button
                if(12hr == 11){12hr = 0; }
                else { 12hr++; }
            }

            if ((PORTE_IN & 0b00000100) == 0) {// check the 'DOWN' button
                if(12hr == 0){12hr = 11; }
                else { 12hr--; }
            }
        }

        while ((PORTE_IN & 0b00000111) != 0b00000111) { // wait for all buttons to be released
            display_digit(12hr/10, 1);
            display_digit(12hr%10, 2);
            display_digit(min/10, 3);
            display_digit(min%10, 4);
        } 
        //``````````````````````````````````````
    }
}