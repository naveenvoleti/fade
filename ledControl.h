/******************************************************************************************************************
 File : ledControl.h
 Author: ye0127
 File Description : ledControl.h is a header file which has all function declaration of LedOn.c functions.
 Created on 29 December, 2020, 3:42 PM
 ******************************************************************************************************************/ 
 void ledOn(int, double, char);						// function declaration for ledOn() function
 void ledOff(void);                                 // function declaration for ledOff() function
 void timerDelay(double);							// function declaration for timerDelay() function
 void brightLED( int);								// function declaration for brightLED() function
 void colorControl(char);							// function declaration for ColorControl() function
 void colorCode(int, int, int);						// function declaration for colorCode() function
 void ledFade_time(unsigned int,unsigned int,unsigned int,unsigned int,char);				// function declaration for led_fade() function
 void ledBlink(int, double, int, int, char);       // function declaration for ledDelay() function
 unsigned int timePeriod(double, unsigned int, unsigned long int);		// function declaration for Period_delay1() function
 int check_valid_value(unsigned int,unsigned int*,char*,char*,char*);