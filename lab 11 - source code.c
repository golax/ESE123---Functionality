#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "functions11.h" // see the rewritten header file ("functions11.h")

float measure_current (void)
{
    float volt; 
    float current;
    // set ADC
    ADC0_CTRLC = 0b01100011;		// small cap, Vref reference, prescale 16
    ADC0_MUXPOS = 0b00001100;		// select AIN13 to measure
    ADC0_COMMAND = 0b00000001;		// start conversion
    while (ADC0_COMMAND)            // wait for completion 
        {}
    volt = (4.3 * ADC0_RES)/(1023.0 * 64.0);    // calculate voltage
    current = volt/2.3   // calculates current

    return (current);
}

int main(void){
    float avdd = 0; // avdd function within the header file, current function in this file
    float current; 
    float set_point;
    setup();

    PORTC_OUT = 0b11011111; // turn on the USB LED, to test the current
    _delay_ms(10000);
    set_point = current * 0.8;

    while(1){
        avdd = 0.9720 * measure_AVDD(); // change the constant to measure more accurately (vs the multimeter thingie)
        current = measure_current();
        for (int i = 0; i < 201; i++){ disp_float(avdd); } // current vs voltage(avdd)
        for (int i = 0; i < 201; i++){ disp_float(current); } // current vs voltage(avdd)
    }
}