// disable watchdog timer
#pragma config PERIOD = PERIOD_OFF, WINDOW = WINDOW_OFF

// set brown-out detect
#pragma config SLEEP = SLEEP_ENABLED, ACTIVE = ACTIVE_ENABLED
#pragma config SAMPFREQ = SAMPFREQ_1KHZ, LVL = LVL_BODLEVEL7

// set clock to 20MHz
#pragma config FREQSEL = FREQSEL_20MHZ, OSCLOCK = SET

// set SYSCFG0 
#pragma config EESAVE = CLEAR, RSTPINCFG = RSTPINCFG_GPIO
#pragma config CRCSRC = CRCSRC_NOCRC

// set SYSCFG1
#pragma config SUT = SUT_64MS

// unlock memory
#pragma config LB = LB_NOLOCK



// defines
#define USBfet	0b00100000
#define dig1	0b00010000
#define dig2	0b00001000
#define dig3	0b00000100
#define dig4	0b00000010
#define leds	0b00000001
#define tick 400
//#define ctick 150
#define ctick 956 // should be around one count per second


// includes
#include <avr/io.h>         // declares variables for the ATmega4809 processor
#include <math.h>           // for the log function
#include <stdlib.h>         // random number functions
#include <avr/eeprom.h>     // eeprom 
#include <avr/cpufunc.h>    // eeprom constants


// setup sets up the microcontroller 
void setup (void)
{
	// setup clock
	CPU_CCP = 0xd8;						// unlock register
	CLKCTRL_MCLKCTRLA = 0b00000000;		// 20MHz clock
	CPU_CCP = 0xd8;						// unlock register
	CLKCTRL_MCLKCTRLB = 0b00010000;		// prescaler disabled

	// porta is used for segment drivers
    PORTA_OUT = 0b11111111; // all LEDs off
	PORTA_DIR = 0b11111111;
    PORTA_PIN0CTRL = 0b10000000; // invert output pin
    PORTA_PIN1CTRL = 0b10000000; // invert output pin
    PORTA_PIN2CTRL = 0b10000000; // invert output pin
    PORTA_PIN3CTRL = 0b10000000; // invert output pin   
    PORTA_PIN4CTRL = 0b10000000; // invert output pin
    PORTA_PIN5CTRL = 0b10000000; // invert output pin
    PORTA_PIN6CTRL = 0b10000000; // invert output pin
    PORTA_PIN7CTRL = 0b10000000; // invert output pin  
    
    // portb is unavailable in the 40 pin dip
    PORTB_PIN0CTRL = 0b00000100; // disable input
    PORTB_PIN1CTRL = 0b00000100; // disable input
    PORTB_PIN2CTRL = 0b00000100; // disable input
    PORTB_PIN3CTRL = 0b00000100; // disable input
    PORTB_PIN4CTRL = 0b00000100; // disable input
    PORTB_PIN5CTRL = 0b00000100; // disable input
       
    // portc is used to drive the digit select transistors and the USB switch
    // portc bits 6 and 7 are not connected in the 40 pin dip and are disabled
    PORTC_PIN6CTRL = 0b00000100; // disable input
    PORTC_PIN7CTRL = 0b00000100; // disable input
    PORTC_OUT = 0b00111111; // all off
    PORTC_DIR = 0b11111111; // unavailable pins output
    
    // portd is unused
    
    // porte is used for switch inputs 
    PORTE_PIN0CTRL = 0b00001000;	// enable pullup
	PORTE_PIN1CTRL = 0b00001000;	// enable pullup
    PORTE_PIN2CTRL = 0b00001000;	// enable pullup
    PORTE_PIN3CTRL = 0b00001000;    // unused pin
	
    // portf is used for the crystal oscillator and one analog input
	PORTF_PIN2CTRL=0b00000100; // disable digital input for current sense
	
	// voltage reference setup
	VREF_CTRLA = 0b00110011;		// 4.3 V ADC reference, 4.3V AC reference
	VREF_CTRLB = 0b00000011;		// keep references enabled
	
	// analog to digital converter setup
	ADC0_CTRLA = 0b10000000;		// run standby enabled, ADC disabled
	ADC0_CTRLB = 0b00000110;		// 64 results accumulated
	ADC0_CTRLC = 0b01000011;		// small capacitor, internal reference, prescale 16
	ADC0_CTRLA = 0b10000001;		// ADC enabled
}




// display_digit takes a decimal number (0-9) in the 'number'
// parameter and displays it on a 7-segment LED digit. The
// digit is specified in the 'digit' parameter and must be in
// the range of 1 (leftmost) to 4 (rightmost). The duration
// parameter sets the display time in microseconds.
void display_digit (char number, char digit)
{
	// turn on the appropriate segments
	switch (number)
	{
		case 0:{PORTA_OUT = 0b11111100; break;}
		case 1:{PORTA_OUT = 0b00101000; break;}
		case 2:{PORTA_OUT = 0b10011110; break;}
		case 3:{PORTA_OUT = 0b00111110; break;}
		case 4:{PORTA_OUT = 0b01101010; break;}
		case 5:{PORTA_OUT = 0b01110110; break;}
		case 6:{PORTA_OUT = 0b11110110; break;}
		case 7:{PORTA_OUT = 0b00101100; break;}
		case 8:{PORTA_OUT = 0b11111110; break;}
		case 9:{PORTA_OUT = 0b01101110; break;}
		case 10:{PORTA_OUT = 0b11101110; break;} // for hex output A
		case 11:{PORTA_OUT = 0b11110010; break;} // for hex output b
		case 12:{PORTA_OUT = 0b11010100; break;} // for hex output C
		case 13:{PORTA_OUT = 0b10111010; break;} // for hex output d
		case 14:{PORTA_OUT = 0b11010110; break;} // for hex output E
		case 15:{PORTA_OUT = 0b11000110; break;} // for hex output F
        case ' ':{PORTA_OUT = 0b00000000; break;}
        case '"':{PORTA_OUT = 0b01001000; break;}
        case '\'':{PORTA_OUT = 0b00001000; break;}
		case '-':{PORTA_OUT = 0b00000010; break;}
        case '.':{PORTA_OUT = 0b00000001; break;}
        case '0':{PORTA_OUT = 0b11111100; break;}
		case '1':{PORTA_OUT = 0b00101000; break;}
		case '2':{PORTA_OUT = 0b10011110; break;}
		case '3':{PORTA_OUT = 0b00111110; break;}
		case '4':{PORTA_OUT = 0b01101010; break;}
		case '5':{PORTA_OUT = 0b01110110; break;}
		case '6':{PORTA_OUT = 0b11110110; break;}
		case '7':{PORTA_OUT = 0b00101100; break;}
		case '8':{PORTA_OUT = 0b11111110; break;}
		case '9':{PORTA_OUT = 0b01101110; break;}
        case '=':{PORTA_OUT = 0b00010010; break;}
		case 'A':{PORTA_OUT = 0b11101110; break;}
        case 'C':{PORTA_OUT = 0b11010100; break;}
		case 'E':{PORTA_OUT = 0b11010110; break;}
		case 'F':{PORTA_OUT = 0b11000110; break;}
        case 'G':{PORTA_OUT = 0b11110110; break;}
        case 'H':{PORTA_OUT = 0b11101010; break;}
		case 'I':{PORTA_OUT = 0b00101000; break;}
        case 'J':{PORTA_OUT = 0b00111000; break;}
		case 'L':{PORTA_OUT = 0b11010000; break;}
        case 'N':{PORTA_OUT = 0b11101100; break;}
        case 'O':{PORTA_OUT = 0b11111100; break;}
		case 'P':{PORTA_OUT = 0b11001110; break;}
		case 'S':{PORTA_OUT = 0b01110110; break;}
        case 'U':{PORTA_OUT = 0b11111000; break;}
        case 'Y':{PORTA_OUT = 0b01111010; break;}
        case 'Z':{PORTA_OUT = 0b10011110; break;}
        case '[':{PORTA_OUT = 0b11010100; break;}
        case ']':{PORTA_OUT = 0b00111100; break;}
        case '_':{PORTA_OUT = 0b00010000; break;}
		case 'b':{PORTA_OUT = 0b11110010; break;}
        case 'c':{PORTA_OUT = 0b10010010; break;}
        case 'd':{PORTA_OUT = 0b10111010; break;}
        case 'h':{PORTA_OUT = 0b11100010; break;}
        case 'i':{PORTA_OUT = 0b00100000; break;}
        case 'l':{PORTA_OUT = 0b00101000; break;}
        case 'n':{PORTA_OUT = 0b10100010; break;}
        case 'o':{PORTA_OUT = 0b10110010; break;}
		case 'r':{PORTA_OUT = 0b10000010; break;}
        case 't':{PORTA_OUT = 0b11010010; break;}
        case 'u':{PORTA_OUT = 0b10110000; break;}
		default:PORTA_OUT = 0b11101111;  // underscore for undefined digits
	}
	// turn on the appropriate digit
	switch (digit)
	{
		case 1:
		{
			PORTC_OUT = 0b11101111;
			break;
		}
		case 2:
		{
			PORTC_OUT = 0b11110111;
			break;
		}
		case 3:
		{
			PORTC_OUT = 0b11111011;
			break;
		}
		case 4:
		{
			PORTC_OUT = 0b11111101;
			break;
		}
	}
    _delay_us (2000);           // wait to see light   
    PORTC_OUT = 0b11111111;     // all digits off
}


// display_hex displays numbers from 0 to FFFF inclusive on the 7 segment display
void display_hex (unsigned int number)
{
	char dig;
	dig = number / 0x1000;				// integer division: get first digit
	display_digit(dig, 1);              // display 16^4 digit
	number %= 0x1000;					// compute remainder
	dig = number / 0x100;				// get second digit
	display_digit(dig, 2);              // display 16^3 digit
	number %= 0x100;					// compute remainder
	dig = number / 0x10;				// gets third digit
	display_digit(dig, 3);              // display 16 digit
	number %= 0x10;						// compute remainder
	display_digit(number, 4);           // display ones digit
}