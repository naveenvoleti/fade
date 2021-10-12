
/*****************************************************************************************************************
 File:   pwm.c
 Author: ye0127
 File description: This is the PWM configuration file , the two functions PWM1_Init() and PWM1_setDC()
 are used to initialize the inbuilt pwm module and generate the PWM wave for user defined brightness value.
 We are generating software PWM wave at three pin to control the Color of LED.  
 Created on 22 April, 2021, 8:51 AM
 ****************************************************************************************************************/
#include <xc.h>
#include <stdio.h>
#include "configbits.h"                                                 // This header file includes configuration bits of pic18f4550
#include "pwm.h"                                                        // This header file includes pwm functions declarations
#include "pins.h"                                                        // This header file includes all the pin declarations


int TMR1_C;
int PWM0DC ,PWM1DC, PWM2DC ;

/*****************************************************************************************************************
 Function : PWM1_Init()
 Function description : PWM1_Init() function is to initialize the PWM module of pic18f4550 using CCP1CON, TMR2 and PR2 
 registers for given pwm period of 155, which generates pwm wave of 2 KHz frequency using internal CCP1 module
 at RC2 pin. [Refer pic18f4550 data sheet for clear description of registers] 
 *****************************************************************************************************************/
void PWM1_Init(int period)
{
    pwm_control_pin = 0;                                         			//  Set RC2 as output    
    /* CCP PWM mode */
    CCP1CONbits.DC1B1 = 0;                              				//  LSB bits of duty cycle zeroed (DC1B1:DC1B0 = 00)
    CCP1CONbits.DC1B0 = 0;
    CCP1CON |= 0x0C;                                         			//  PWM mode ( CCP1M3:CCP1M0 = 1100)    
    /* Timer2 configuration */
    PR2 = period;                                                  		// configure timer2 period at 2KHz pwm frequency
    T2CON = 0x02;                                              			//  Set prescalar 1:16   
    TMR2ON = 1;                                                  		//  timer2 on    
}

/*****************************************************************************************************************
 Function : PWM1_setDC()
 Function description : PWM1_setDC() function is to configure the duty_cycle of the pwm wave for given brightness using
 internal PWM module.
  *****************************************************************************************************************/
void PWM1_setDC(int dutycycle)
{
   // unsigned int Duty = [PR2+1]*(dutycycle/100);
    CCPR1L = dutycycle>>2;                               				//  PWM duty cycle - first 8-bits (MSb)
    CCP1CON &= 0xCF;                                          			//  5,4 bits zeroed (DC1B1:DC1B0 = 00) with single output
    CCP1CON |= ((dutycycle<<4)&0x30);           						//  PWM duty cycle - last 2-bits (LSb) in CCP1CON 5,4 bits  
}


/*****************************************************************************************************************
 Function description : This is Interrupt Service Routine used to generate the software PWM with given Color code 
 values. For RGB LED, each color has three values in particular ratio [Eg: RED (0,255,255). Based on this value 
 three software pwm waves are generated to glow the LED with user given Color.
 internal PWM module.
  *****************************************************************************************************************/
void __interrupt() ISR()
{
    if(TMR1IF) 
    {
        TMR1_C++;                                                      // Increment the timer count for each interrupt
        if(TMR1_C>=PWM0DC)                                             // Compare timer count value with duty cycle1
        {
            PWM0 = 0;                                                  // Drive PWM0 Output LOW
        }
        if(TMR1_C>=PWM1DC)                                             //  Compare timer count value with duty cycle2
        {
            PWM1 = 0;                                                  // Drive PWM1 Output LOW
        }
        if(TMR1_C>=PWM2DC)                                             // Compare timer count value with duty cycle2
        {
            PWM2 = 0;                                                  // Drive PWM2 Output LOW
        }
        if(TMR1_C==255)
        {
            PWM0 = 1; 
            PWM1 = 1;                                                  // Drive all PWM Outputs to HIGH
            PWM2 = 1;
            TMR1_C = 0;                                                // Reset Counter
        }
        TMR1IF = 0;                                                    // Clear the Timer1 interrupt flag and load the initial value
        TMR1 = 65340;                                                  // pwm frequency at 100Hz
    }
}

/*****************************************************************************************************************
 Function : TMR1_Init()
 Function description : TMR1_Init() function is used to generate the Interrupt Service Routine using Timer1 module 
*****************************************************************************************************************/
void TMR1_Init(void)
{
    TMR1 = 65340;														// Load the Value to TMR1 register
    TMR1CS = 0;                                                         // local clock source (timer mode)
    T1CKPS0 = 0;                                                        //  pre-scaler ratio (1:1)
    T1CKPS1 = 0;
    TMR1ON = 1;                                                         // Switch ON Timer1 Module
    /* Interrupts Configurations ]*/
    TMR1IE = 1;                                                         // Timer1 Interrupt Enable Bit
    TMR1IF = 0;                                                         // Clear The Interrupt Flag Bit
    PEIE = 1;                                                           // Peripherals Interrupts Enable Bit
    GIE = 1;                                                            // Global Interrupts Enable Bit
}


