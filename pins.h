/*****************************************************************************************************************
 File:   pins.c
 Author: ye0146
 File description: This is the PIN header file which contains all the necessary pin declarations of the LEDS.  
 Created on 22 April, 2021, 8:51 AM
 ****************************************************************************************************************/

#define _XTAL_FREQ 20000000                     						// External oscillator frequency of the micro controller

/* Software PWM pins */
#define PWM0_D TRISD0
#define PWM0 PORTDbits.RD0                                              // RedPin of the Led is connected 
#define PWM1_D TRISD1
#define PWM1 PORTDbits.RD1                                              // GreenPin of the Led is connected
#define PWM2_D TRISD2
#define PWM2 PORTDbits.RD2                                              // BluePin of the Led is connected

#define pwm_control_pin TRISCbits.RC2                                   // PWM controlpin is connected to this pin(internal CCP1 pin for PWM generation)

#define commonAnode 0                                                  // Led Type 1 = common anode, 0 = common cathode