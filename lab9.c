#define F_CPU 2000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "functions.h"

int main(void)
{
    int number = 6837;
    int disp_int; // a 'scratch' variable
    setup(); // a setup function to prepare the microcontroller
    while(1){

        disp_int = number;
        // display_digit(value, location)
        
        display_digit(disp_int/1000, 1);
        disp_int = disp_int % 1000;

        display_digit(disp_int/100, 2);
        disp_int = disp_int % 100;

        display_digit(disp_int/10, 3);

        display_digit(disp_int % 10, 4);
    }
}
