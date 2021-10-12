
/********************************************************************************************************************************
  File:   RGBmain.c
  Author: ye0127
  File description: This is the main function or template of the Single RGB LED project, which accepts all the required parameters
  for LED functionalities which includes i) LED ON ii) LED OFF iii) LED Blink with user required Color.  The possible Colors are
  R:Red, L:Lime, B:Blue, G:Green, M:Maroon, C:Cyan, S:Silver, Y:Yellow, P:Purple, N:Navy, T:Teal, O:Olive and F:Magenta / Fuchsia. 
  All these functions are called by passing required parameters to enable a particular action for Single RGB LED from this file.
  Created on 29 December, 2020, 2:48 PM
*********************************************************************************************************************************/

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "configbits.h" 						// This header file includes configuration bits of pic18f4550
#include "pins.h"                               // This header file includes all the pin declarations
#include "ledControl.h"						    // This header file includes function declarations of LedOn.c
#include "structure.h"

void RunLed(RGB_LED LED1);
/*******************************************************************************************************************************
  The main() function accepts all the user given values and assign them to respective structure members. For LED Eight parameters 
  [Device_id,Device,action Required, Count, Color,Brightness, Period,fadeAmount, OnTime, OffTime ] are needed.
********************************************************************************************************************************/
void main(void) 
{
    TRISD = 0X00;                                                                        // Software pwm i/o pins direction set to zero
    PORTDbits.RD4 = 1;
    TRISC2= 0;                                                                           // TRISC2 direction to output
    PORTCbits.RC2 = 0;
    
    RGB_LED LED1; 					                            				         // defines Structure variable of RGB led      
                                                                                    
    //strcpy(LED1.Device_id,"ATM0001");          					 // loading location of LED, for instance "None
    //strcpy(LED1.Device,"LED0001");                       				 // copying device type value LED0001 into Device_type variable using strcpy
    strcpy(LED1.action_req,"LEDFADE");                            // for instance we are considering LEDON action as input
    
    //LED1.Count = 5;                                                                      // Single LED
    //LED1.Color = 'R';                                                                    // load the required color
    //LED1.Brightness = 99;                                                                // Load required brightness, default is 100
    //LED1.fadeAmount = 1;
    //LED1.Period = 10;                                                                    // Total time period in sec to control the LED
    //LED1.OnTime = 1;                                                                     // Delay time for LED on, 1 for continuous ON   
    //LED1.OffTime = 2;																	 // Delay time for LED off, 1 for continuous Off   
    LED1.positive_fade_rate = 5;
    LED1.negative_fade_rate = 5;
    LED1.fadeIn_min_brightness = 1;
    LED1.fadeIn_max_brightness = 100;
    LED1.fadeOut_min_brightness = 1;
    LED1.fadeOut_max_brightness = 100;
    //ledFade(LED1.Brightness, LED1.Period,LED1.fadeAmount, LED1.Color);
    //ledBlink(LED1.Brightness, LED1.Period, LED1.OnTime,  LED1.OffTime, LED1.Color);
    //Device_actions(LED1);                                                                // Passing structure variable LED1 to the function
    RunLed(LED1);
 while(1);
}

/*************************************************************************************************************************************************
 Function : Device_actions()
 Function description : Device_actions() performs the user defined LED action. This function checks the individual parameters with user inputs,
 if all parameters are valid it performs the required LED actions. The conditions to perform individual LED actions are as follows
	* LEDOn : if OnTime = 1, OffTime = 0, LEDon action is performed with user given Timeperiod, Brightness and Color
	* LEDOff : if OnTime = 0, OffTime = 1, LEDOff action is performed
	* LEDBlink : if OnTime > 1, OffTime > 1, LEDBlink action is performed with user given Ontime, Offtime, Total Timeperiod, Brightness and Color
 *************************************************************************************************************************************************/

//void Device_actions(struct singleLed LED1)
//{
//    int res = strcmp(LED1.Device_id, "ATM0001");                                
//    int res1 = strcmp(LED1.Device, "LED");
//    if(res == 0)                                                                                        // Checks the device_type, default is "None"
//    {
//      if(res1 == 0)                                                                                     // Checks the device , default "LED"
//      {
//           if (LED1.Count == 1)                                                                 		// Checks the led count, default 1
//           {
//               if(LED1.Color == 'W')                                                            	    // Checks the led color, default color is white 'w'
//               {
//                  if(LED1.Brightness <= 100)                                               			    // Checks the brightness, default 100%
//                  {
//                      if(LED1.OnTime == 1 && LED1.OffTime == 0)          
//                      {
//                            ledOn(LED1.Brightness, LED1.Period, LED1.Color);               				// Calling  continuous on functionality with given period of time      
//                      }
//                      else if(LED1.OnTime == 0 && LED1.OffTime == 1)
//                      {
//                            ledOff();                                               					//Calling continuous off functionality
//                       }
//                       else if(LED1.OnTime != 0 && LED1.OffTime != 0)
//                       {
//                            ledBlink(LED1.Brightness, LED1.Period, LED1.OnTime,  LED1.OffTime, LED1.Color);              //Calling LED blinking functionality
//                       }
//                       else {
//                       // return error message as required parameters are not available 
//                       }
//                    }
//                 /* else if(LED1.Brightness > 100  && LED1.Brightness < 0)                                       
//                  {
//                  error message : invalid brightness value
//                    }*/
//                  }
//               else if(LED1.Color != 'W')                                                         		// Checks the led color, enter inside the loop if color is not white  
//               {  
//                    if(LED1.Brightness <= 100)                                                			// Checks the brightness, default 100%
//                    {
//                       if(LED1.OnTime == 1 && LED1.OffTime == 0)          
//                       {
//                           ledOn(LED1.Brightness, LED1.Period,LED1.Color);                              // Calling  continuous on functionality
//                       }
//                       else if(LED1.OnTime == 0 && LED1.OffTime == 1)
//                       {
//                             ledOff();                                            						// Calling  continuous off functionality
//                       }
//                        else if(LED1.OnTime != 0 && LED1.OffTime != 0)
//                        {
//                             ledBlink(LED1.Brightness, LED1.Period, LED1.OnTime,  LED1.OffTime, LED1.Color);       //Calling continuous off functionality
//                        }
//                        else{}
//                    }
//                 }
//               }
//            }         
//         }
//}    
