#define F_CPU 2000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "functions.h"

int main(void)
{
    int number = 0;
    char run = 0; // 0 = off, 1 = on
    int disp_int; // a 'scratch' variable - modifying the value of 'number' as a copy
    setup(); // a setup function to prepare the microcontroller

    while(1){
        disp_int = number;
        // display_digit(value, location)
        display_digit('.', 2); // decimal point
        
        display_digit(disp_int/1000, 1); // integer-dividing 'disp_int' by 1000 will give the thousands place value only
        disp_int = disp_int % 1000; // modulo 1000 will remove the thousands place
        display_digit(disp_int/100, 2); // int-divide by 100 for hundreds place value
        disp_int = disp_int % 100; // modulo 100 will remove the hundreds place
        display_digit(disp_int/10, 3); // int-divide by 10 for tens place value
        display_digit(disp_int % 10, 4);// *modulo* by 10 will give the ones place value

        if (run == 1){ 
                if (number == 9999){ number = 0;}
                else { number++; } 
            }
        if ((PORTE_IN & 0b00000001) == 0){ run = 1; } // up button function
        if ((PORTE_IN & 0b00000010) == 0){ run = 0; } // ok button function
        if ((PORTE_IN & 0b00000100) == 0){ number = 0; } // down button function
    }
}