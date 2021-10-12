/******************************************************************************************************************
 File : pwm.h
 Author: ye0127
 File Description : pwm.h is a header file which has all function declaration of pwm.c functions.
 Created on 29 December, 2020, 3:35 PM
 ******************************************************************************************************************/ 

 void PWM1_Init(int);                        // Initialize PWM module 
 void PWM1_setDC( int);                  	 // set the PWM Duty cycle
 void TMR1_Init(void);                       // Initialize timer1 module for software pwm module