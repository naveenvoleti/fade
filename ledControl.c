
/*****************************************************************************************************************
 File: LedOn.c
 Author: ye0127
 File description: LedOn.c is library file which has functionalities to TurnOn the LED with user defined Color,
 Brightness and Time period. 
Created on 29 December, 2020, 2:48 PM
 ****************************************************************************************************************/
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "configbits.h"                                                 // This header file includes configuration bits of pic18f4550
#include "pwm.h"                                                        // This header file includes pwm functions declarations
#include "pins.h" 
#include "ledControl.h"// This header file includes all the pin declarations
#include "structure.h"
#include "properties.h"

int TMR1_C;
int PWM0DC ,PWM1DC, PWM2DC ;

 void led_operation(struct valid_data valid_string);
 char*  getValue(char* ,char* );
 
 //void check_valid_value(int, int);
/************************************************************************************************************************/
/* Function Name : check__valid_input()                                                                                                                                                                  */
/* Function description: The check_valid_input() function takes structure variable 'singleLed LED' as an input.               */
/* The structure variable has all nine required parameters for LED functions. Each parameter value is validated with       */
/* its Size [Device_id], minimum and maximum values[num_LED,Brightness, Total_time, OnTime, OffTime]. If all parameter*/
/* values are valid then req_function()is called with updated parameter values to perfrom the user defined LED              */
/* action [LEDON, LEDOFF, LEDBLINK,LEDFADE].If the parameter is invalid then specific error code will display. If the parameter   */
/* value is NULL, then default values  * of specific parameter is loaded from property file and req_function()is called      */
/* to perform the user defined LED action [LEDON, LEDOFF, LEDBLINK,LEDFADE] with default values.                                                   */
/************************************************************************************************************************/
 void RunLed(RGB_LED LED) {
     int error;
    /* Validating Device_id */
    if (strcmp(LED.Device_id, NULL) == 0) //  if no parameter value for Device_id from user then this block reads default value from property file 
    {
        char* config_val; // local variables used to store the output of getValue() function
        char val[25];
        char def_devid[8];
        config_val = getValue(default_devid, val); // calling getvalue() function to load the default parameter value of Device_id from property file
        for (int r = 0; r < 8; r++) {
            //char def_devid[8];
            def_devid[r] = config_val[r + 1];
        }
        strcpy(valid_string.Devid, def_devid); // copying default Device_id value into new structure variable 
    }
        /*  if there is a user input value in Device_id then, the below block validate the input Device-id value in terms of
            length, number of characters and number of digits with pre-defined property file value */
    else {
        /* calculating length of the device_id, number of characters and number of digits in user input*/
        unsigned int id_len = strlen(LED.Device_id); // calculating length of Device_id using strlen() predefined function
        int characters_len = 0; // variable to store the number of characters in a string
        int num_len = 0; // variable to store the number of digits in a string
        for (int i = 0; i < 8; i++) {
            // Separating number of characters from Device_id 
            if (((LED.Device_id[i] >= 'A' && LED.Device_id[i] <= 'Z')) || ((LED.Device_id[i] >= 'a' && LED.Device_id[i] <= 'z')))
                characters_len++;
                // Separating numbers of digits from Device_id
            else if (isdigit(LED.Device_id[i]))
                num_len++;
        }
        char* config_val; // local variables used to store the output of getValue() function
        char val[20];
        char* devid_len; // to store the output of strtok() function
        char* sep_token = ",";
        //char* cfg_devlen;char* cfg_devlen1; char* cfg_devlen2;        		      // variables to store individual values after split operation
        int i = 1, count = 0;
        int DevidLen, Devidnum, Devidlet; // to store length, no of characters and no of digits of Device_id		
        config_val = getValue(devid_length, val); // calling getvalue() function to read the Devid_len value from property file  
        devid_len = strtok(config_val + 1, sep_token); // Separating length, no of characters and no of digits using strtok() pre-deined 									  	      // function                           
        while (devid_len != NULL) {
            if (count == 0) { // Separating length of Device_id        
                DevidLen = atoi(devid_len); // converting ascii value to integer value using atoi() pre-defined function
            } else if (count == 1) { // Separating num of characters in Device_id from property file
                Devidlet = atoi(devid_len); // converting ascii value to integer value using atoi() pre-defined function
            } else if (count == 2) {
                Devidnum = atoi(devid_len); // converting ascii value to integer value using atoi() pre-defined function
            }
            count++;
            devid_len = strtok(NULL, sep_token);
        }
        if ((id_len == DevidLen) && (characters_len == Devidlet) && (num_len == Devidnum)) {
            char* config_val; // local variables used to store the output getValue() function
            char val[50];
            char* devid_res;
            char* sep_token = ",";
            char* cfg_devid;
            char* cfg_devid1;
            char* cfg_devid2;
            int i = 1, count = 0;
            char devid1[8], devid2[8], devid3[8];
            config_val = getValue(devid_value, val); // calling getvalue() function to read the Device_id value from property file
            devid_res = strtok(config_val + 1, sep_token); // separating each Device_id values using strtok() pre-defined function 
            while (devid_res != NULL) {
                if (count == 0) { // separating first Device_id value from property file
                    cfg_devid = devid_res;
                    for (int i = 0; i < 8; i++) {
                        devid1[i] = cfg_devid[i];
                    }
                } else if (count == 1) { // separating second Device_id value from property file
                    cfg_devid1 = devid_res;
                    for (int j = 0; j < 8; j++) {
                        devid2[j] = cfg_devid1[j];
                    }
                } else if (count == 2) { // separating third Device_id value from property file
                    cfg_devid2 = devid_res;
                    for (int k = 0; k < 8; k++) {
                        devid3[k] = cfg_devid2[k];
                    }
                }
                count++;
                devid_res = strtok(NULL, sep_token);
            }
            if (strcmp(LED.Device_id, devid1) == 0) // comparing user input Device_id with property file value using strcpy() 
                strcpy(valid_string.Devid, LED.Device_id); // pre-defined function, if both are equal copy the Device_id value into
                // new strcture variable
            else if (strcmp(LED.Device_id, devid2) == 0) // comparing user input Device_id with property file value using strcpy() 
                strcpy(valid_string.Devid, LED.Device_id); // pre-defined function, if both are equal copy the Device_id value into
                // new strcture variable
            else if (strcmp(LED.Device_id, devid3) == 0) // comparing user input Device_id with property file value using strcpy() 
                strcpy(valid_string.Devid, LED.Device_id); // pre-defined function, if both are equal copy the Device_id value into
                // new strcture variable
            else {
                // Error code : invalid device_id                    		// if not match, throw the Error code
            }
        }
        else {
            // Error Code : invalid device_id
        }
    }

    /* Validating num_LED */

    /* if no parameter value for num_LED from user then this block reads default value from property file */
    
    if (LED.Count == 0) {
        char* config_val; // local variables used to store the output of getValue() function
        char val[10];
        char default_cnt[5];
        config_val = getValue(default_num_LED, val); // load the default parameter value of num_LED from property file
        for (int j = 0; j < strlen(config_val); j++) {
            default_cnt[j] = config_val[j + 1];
        }
        int LED_default = atoi(default_cnt); // converting ascii value to integer value using atoi() pre-defined function
        valid_string.numLED = LED_default; // copying default num_LED value into new structure variable 
    }
        /* if there is a value in num_LED , the below block of code checks whether the input in number or not, if it is a 
           number, read the maximum and minimum values from property file and compare with user input num_LED */
        /* isdigit() is a pre-defined function, which checks if the input is number or not, it return zero if input is number */

    else {
        if (isdigit(LED.Count) == 0) { // check if num_LED is number or not, if it is number isdigit() returns zero
            /* get min and max values for num_LED from property file and compare with the user given num_LED */
            char* config_val;
            char* config_val1;
            char val[10];
            char min_cnt[5], max_cnt[10];
            int LED_min, LED_max;
            config_val = getValue(num_LED_MIN, val); // Calling getvalue() function to read the minimum value from property file
            for (int i = 0; i < 5; i++) {
                min_cnt[i] = config_val[i + 1];
            }
            LED_min = atoi(min_cnt); // converting ascii value to integer value using atoi() pre-defined function
            config_val1 = getValue(num_LED_MAX, val); // Calling getvalue() function to read the maximum value from property file
            for (int j = 0; j < 10; j++) {
                max_cnt[j] = config_val[j + 1];
            }
            LED_max = atoi(max_cnt); // converting ascii value to intiger value using atoi() pre-defined function
            if (LED.Count >= LED_min && LED.Count < LED_max) // checking user given num_LED value with min and max values
            {
                valid_string.numLED = LED.Count; // copying user given num_LED value into new structure variable 
            } else {
                //Error code : invalid num_LED
            }
        }
        else {
            // Error Code : invalid num_LED (number of LEDs)
        }
    }
    /* Validating Color */

    /*  if there is no parameter value for Color from user then this block reads default value from property file */
    if (LED.Color == NULL) {
        char* config_val; // local variables used to store the output of getValue() function
        char val[10];
        config_val = getValue(default_Color, val); // Calling getvalue() function to load the default parameter value of Color from property file 											// property file
        valid_string.color = config_val[1];
    }        /* if there is input value, below block of code checks whether input is a character or not, if it is character, then
       checks if it is in upper case or not, if the input is not in upper case convert the input into upper case letter */
    else {
        if (isalpha(LED.Color) > 0) // checks if user input is character or not
        {
            /* read all possible color from property file and compare with user given color type */
            if (isupper(LED.Color) == 0) // if the user input color is not in upper case, convert it into upper case letter
                LED.Color = toupper(LED.Color);
            char* config_val; // local variables used to store the output getValue() function
            char val[10];
            char* color_res;
            char* sep_token = ",";
            char cfg_color1;
            char cfg_color2;
            char cfg_color3;
            char cfg_color4;
            char cfg_color5;
            char cfg_color6;
            char cfg_color7;
            char cfg_color8;
            char cfg_color9;
            char cfg_color10;
            char cfg_color11;
            char cfg_color12;
            char cfg_color13;
            char cfg_color14;
            int i = 1, count = 0;
            config_val = getValue(Color, val); // Calling getvalue() function to read the values from property file
            color_res = strtok(config_val + 1, sep_token); // Separating each color value usin strtok() pre-defined function
            while (color_res != NULL) {
                if (count == 0) { // Separating first color value from property file
                    cfg_color1 = color_res[0];
                } else if (count == 1) { // Separating second color value from property file
                    cfg_color2 = color_res[0];
                } else if (count == 2) { // Separating third color value from property file
                    cfg_color3 = color_res[0];
                } else if (count == 3) { // Separating second color value from property file
                    cfg_color4 = color_res[0];
                } else if (count == 4) { // Separating third color value from property file
                    cfg_color5 = color_res[0];
                } else if (count == 5) { // Separating second color value from property file
                    cfg_color6 = color_res[0];
                } else if (count == 6) { // Separating third color value from property file
                    cfg_color7 = color_res[0];
                } else if (count == 7) { // Separating second color value from property file
                    cfg_color8 = color_res[0];
                } else if (count == 8) { // Separating third color value from property file
                    cfg_color9 = color_res[0];
                } else if (count == 9) { // Separating third color value from property file
                    cfg_color10 = color_res[0];
                } else if (count == 10) { // Separating second color value from property file
                    cfg_color11 = color_res[0];
                } else if (count == 11) { // Separating third color value from property file
                    cfg_color12 = color_res[0];
                } else if (count == 12) { // Separating second color value from property file
                    cfg_color13 = color_res[0];
                } else if (count == 13) { // Separating third color value from property file
                    cfg_color14 = color_res[0];
                }
                count++;
                color_res = strtok(NULL, sep_token);
            }
            if (LED.Color == cfg_color1)
                valid_string.color = LED.Color; // copying user given Color value into new structure variable 
            else if (LED.Color == cfg_color2)
                valid_string.color = LED.Color; // copying user given Color value into new structure variable 
            else if (LED.Color == cfg_color3)
                valid_string.color = LED.Color; // copying user given Color value into new structure variable
            else if (LED.Color == cfg_color4)
                valid_string.color = LED.Color; // copying user given Color value into new structure variable 
            else if (LED.Color == cfg_color5)
                valid_string.color = LED.Color;
            else if (LED.Color == cfg_color6)
                valid_string.color = LED.Color; // copying user given Color value into new structure variable 
            else if (LED.Color == cfg_color7)
                valid_string.color = LED.Color;
            else if (LED.Color == cfg_color8)
                valid_string.color = LED.Color; // copying user given Color value into new structure variable 
            else if (LED.Color == cfg_color9)
                valid_string.color = LED.Color;
            else if (LED.Color == cfg_color10)
                valid_string.color = LED.Color;
            else if (LED.Color == cfg_color11)
                valid_string.color = LED.Color; // copying user given Color value into new structure variable 
            else if (LED.Color == cfg_color12)
                valid_string.color = LED.Color;
            else if (LED.Color == cfg_color13)
                valid_string.color = LED.Color; // copying user given Color value into new structure variable 
            else if (LED.Color == cfg_color14)
                valid_string.color = LED.Color;
            else {
                // Error code : invalid device_id
            }
        }
        else {
            // Error Code : invalid Color
        }
    }

    /* Validating Brightness */

    /* if there is no input for Brightness, below block of code loads default value from property file*/
    error = check_valid_value(LED.Brightness,&valid_string.Brts,default_Brightness,Brightness_MIN,Brightness_MAX);
    
    /* Validating Total_time */
    /* if there is no input value for Total_time, below block of code loads the default value of Total_time from property file */
    error = check_valid_value(LED.Period,&valid_string.Totaltime,default_Total_time,Total_time_MIN,Total_time_MAX);
    
    /* Validating OnTime */
    /* if there is no input value for OnTime, below block of code loads the default value of OnTime from property file */
    error = check_valid_value(LED.OnTime,&valid_string.OnTime,default_OnTime,OnTime_MIN,OnTime_MAX);
    /* Validating OffTime */
    /* if there is no input value for OffTime, below block of code loads the default value of OffTime from property file */
    error = check_valid_value(LED.OffTime,&valid_string.OffTime,default_OffTime,OffTime_MIN,OffTime_MAX);
    
    /* Validating FadeTime */
    /* if there is no input value for FadeTime, below block of code loads the default value of OffTime from property file */
    error = check_valid_value(LED.positive_fade_rate,&valid_string.positive_fade_amount,default_positiveFade,positiveFade_MIN,positiveFade_MAX);
    
    error = check_valid_value(LED.negative_fade_rate,&valid_string.negative_fade_amount,default_negativeFade,negativeFade_MIN,negativeFade_MAX);
    
    error = check_valid_value(LED.fadeIn_min_brightness,&valid_string.fadeIn_min_brt,default_FadeInBrightness,FadeInBrightness_MIN,FadeInBrightness_MAX);
    
    error = check_valid_value(LED.fadeIn_max_brightness,&valid_string.fadeIn_max_brt,default_FadeInBrightness,FadeInBrightness_MIN,FadeInBrightness_MAX);
    
    error = check_valid_value(LED.fadeOut_min_brightness,&valid_string.fadeOut_min_brt,default_FadeOutBrightness,FadeOutBrightness_MIN,FadeOutBrightness_MAX);
    
    error = check_valid_value(LED.fadeOut_max_brightness,&valid_string.fadeOut_max_brt,default_FadeOutBrightness,FadeOutBrightness_MIN,FadeOutBrightness_MAX);
    /* Validating action_req */
    /* if there is no input for action_req, below block of code loads the default value from property file */
    if (strcmp(LED.action_req, NULL) == 0) {
        char* config_val;
        char action[10];
        char def_action[10];
        config_val = getValue(default_action_req, action); // Calling getvalue() function to load the default parameter value of action_req from 											// property file
        for (int i = 0; i < 10; i++) {
            def_action[i] = config_val[i+1];
        }
        strcpy(valid_string.actreq, def_action);
    }
        /* if there is input value, below block of code executes*/
    else {
        char* config_val;
        char val[10];
        unsigned int action_len;
        int max_length; // local variable to store the max action length from property file
        char act_len[5];
        action_len = strlen(LED.action_req); // calculating length of user input action_req
        config_val = getValue(action_length, val); // Calling getValue() function to read action_length_max from property file
        for (int j = 0; j < 5; j++) {
            act_len[j] = config_val[j + 1];
        }
        int max_act_length = atoi(act_len); // converting ascii value to integer value using atoi() pre-defined function
        if (action_len <= max_act_length) {
            char* config_val;
            char action[8];
            char* action_res;
            char* sep_token = ",";
            char* cfg_action;
            char* cfg_action1;
            char* cfg_action2;
            char* cfg_action3;
            int i = 1, count = 0;
            char action1[10], action2[10], action3[10], action4[10]; // to store the individual action
            config_val = getValue(action_req, action); // Calling getvalue() function to read the values of action_req from property file
            action_res = strtok(config_val + 1, sep_token); // Separating each action with strtok() pre-defined function
            while (action_res != NULL) {
                if (count == 0) { // Separating first action from property file
                    cfg_action = action_res;
                    for (int i = 0; i < action_len; i++) {
                        action1[i] = cfg_action[i];
                    }
                } else if (count == 1) {
                    cfg_action1 = action_res;
                    for (int j = 0; j < action_len; j++) {
                        action2[j] = cfg_action1[j];
                    }
                } else if (count == 2) {
                    cfg_action2 = action_res;
                    for (int k = 0; k < action_len; k++) {
                        action3[k] = cfg_action2[k];
                    }
                    
                }else if (count == 3) {
                    cfg_action3 = action_res;
                    for (int l = 0; l < action_len; l++) {
                        action4[l] = cfg_action3[l];
                    }  
                }
                count++;
                action_res = strtok(NULL, sep_token);
            }
            if (strcmp(LED.action_req, action1) == 0) // comparing user input action_req with property file value using strcpy() 
                strcpy(valid_string.actreq, LED.action_req); // pre-defined function, if both are equal copy the action_req value
                // into new structure variable
            else if (strcmp(LED.action_req, action2) == 0) // comparing user input action_req with property file value using strcpy() 
                strcpy(valid_string.actreq, LED.action_req); // pre-defined function, if both are equal copy the action_req value
                // into new structure variable
            else if (strcmp(LED.action_req, action3) == 0) // comparing user input action_req with property file value using strcpy() 
                strcpy(valid_string.actreq, LED.action_req); // pre-defined function, if both are equal copy the action_req value
            else if (strcmp(LED.action_req, action4) == 0) // comparing user input action_req with property file value using strcpy() 
                strcpy(valid_string.actreq, LED.action_req); // pre-defined function, if both are equal copy the action_req value
                // into new structure variable
            else {
                // Error code : invalid action
            }
        } else {
            // Error code:  Invalid action 
        }
    }
      led_operation(valid_string); // caliing req_function() to perform the user defined LED functionality

}
 

 /***********************************************************************************************************************
Function Name : req_function()                                                                                                 
Function description: The req_function() selects which functionality [ledOn(), ledOff(), led_blink()] has to be called. 

***********************************************************************************************************************/

void led_operation(struct valid_data valid_string) {
    unsigned int action_len = strlen(valid_string.actreq);
    if (strncmp(valid_string.actreq, "LEDON",action_len) == 0) {
        ledOn(valid_string.Brts, valid_string.Totaltime,valid_string.color); // Calling LED on functionality 
    } else if (strncmp(valid_string.actreq, "LEDOFF",action_len) == 0) {
        ledOff(); // Calling LED off functionality
    } else if (strncmp(valid_string.actreq, "LEDBLINK",action_len) == 0) {
       ledBlink(valid_string.Brts, valid_string.Totaltime, valid_string.OnTime, valid_string.OffTime,valid_string.color); // Calling LED blink functionality 
    } 
    else if (strncmp(valid_string.actreq, "LEDFADE",action_len) == 0) {
        ledFade_time(valid_string.fadeIn_min_brt,valid_string.fadeIn_max_brt,valid_string.fadeOut_min_brt,valid_string.fadeOut_max_brt,valid_string.color); // Calling LED Fade functionality 
    }
    else {
        /* Error message : invalid function type */
    }
}
int check_valid_value(
         unsigned int input_value, 
         unsigned int* valid_value, 
         char* default_value, 
         char* min_value, 
         char* max_value){
   /* Validating OffTime */
    /* if there is no input value for OffTime, below block of code loads the default value of OffTime from property file */
    if (input_value == 0) {
        char* config_val; // local variables used to store the output of getValue() function
        char cfg_off[5], def[5];
        config_val = getValue(default_value, cfg_off); // Calling getvalue() function to load the default parameter value of OffTime from property file
        for (int j = 0; j < 5; j++) {
            def[j] = config_val[j + 1];
        }
        int off_default = atoi(def); // converting ascii value to integer value using atoi() pre-defined function
        *valid_value = off_default; // copying default OffTime value into new structure variable 
    }        /* if there is input value, below block of code checks whether the input is number or not, if it is a number, 
       read the minimum and maximum values from property file and compare with user input OffTime */
        /* isdigit() is a pre-defined function, which checks whether the input is number or not, it return zero if input is number */
    else {
        if (isdigit(input_value) == 0) {
            char* config_val;
            char* config_val1; // local variables used to store the output of getValue() function
            char cfg_off[10];
            char min_val[5], max_val[5];
            int off_min, off_max;
            config_val = getValue(min_value, cfg_off); // Calling getvalue() function to read min value of OffTime from property file          
            for (int i = 0; i < 5; i++) {
                min_val[i] = config_val[i + 1];
            }
            off_min = atoi(min_val); // converting ascii value to integer value using atoi() pre-defined function
            config_val1 = getValue(max_value, cfg_off); // Calling getvalue() function to read max value of OffTime from property file
            for (int j = 0; j < 5; j++) {
                max_val[j] = config_val1[j + 1];
            }
            off_max = atoi(max_val); // converting ascii value to integer value using atoi() pre-defined function
            if (input_value >= off_min && input_value <= off_max) // if the user given input is between the min and max values
            {
                *valid_value = input_value; // copying user given OffTime value into new structure variable 
            } else {
                char* config_val; // local variables used to store the output of getValue() function
                char cfg_off[5], def[5];
                config_val = getValue(default_value, cfg_off); // Calling getvalue() function to load the default parameter value of OffTime from property file
                for (int j = 0; j < 5; j++) {
                    def[j] = config_val[j + 1];
                }
                int off_default = atoi(def); // converting ascii value to integer value using atoi() pre-defined function
                *valid_value = off_default; // copying default OffTime value into new structure variable 
            }
        }
        else {
            // Error Code : invalid OffTime
        }
    }  
 }

 /*****************************************************************************************************************/
/* Function Name : getValue()                                                                                                                                                 */
/* Function Description : getValue() function is used to separate the Key and values of each parameter from       */
/* property file. The function first checks whether the key and value is present or not, then separate the values  */
/* with = (is equal to) operator and send the result back to called function                                                                    */

/*****************************************************************************************************************/
char* getValue(char* Cfg_value, char* val) {
    if (Cfg_value != NULL) {
        if ((Cfg_value = strchr(Cfg_value, '=')) != NULL) {
            unsigned int len = strlen(Cfg_value);
            if (len > 0) {
                strncpy(val, Cfg_value, len);
                val[len] = '\0';
            }
            return val;
        }
    }
}
/***********************************************************************************************************************
 Function : ledOn()
 Function description : ledOn() function takes three input which are Brightness, Period, color, and turn on the LED 
 based on the Color, Brightness and Timeperiod. 
 **********************************************************************************************************************/
void ledOn(int Brightness,double Period, char color)                                              
{ 
    TMR1_Init();                                                         // Initialize timer1 for software pwm
    brightLED(Brightness);                                               // Turn ON LED with user defined brightness
    colorControl(color);                                                 // Calling color control function with user defined color
    timerDelay(Period);                                                 // Delay function
    brightLED(0);                                                        // As we are controlling LED with pwm output, manually turning off the LED by sending duty-cycle as 0
    colorCode(255,255,255);                                                                               
}

/***********************************************************************************************************************
 Function : ledOff()
 Function description : ledOff() function is used to turn off the LED
 **********************************************************************************************************************/
void ledOff()
{
    /* As we are controlling LED with pwm output,
     manually turning off the LED by sending duty-cycle as 0*/
    TMR1_Init();
    brightLED(0);     
}

/***********************************************************************************************************************
 Function : ColorControl()
 Function description : ColorControl() function takes input as Color, and check the Color with each case. If the Color
 matches it calls colorCode() function by sending Color values as input.  
 **********************************************************************************************************************/
void colorControl(char color)
{
    TRISD = 0X00;
    PORTDbits.RD3 = 1;
    switch(color){
        case 'R' :                                                              
        colorCode(0,255,255);                                            // Red Color
        break;
        case 'L' :
        colorCode(255,0,255);                                            // Lime Color
        break;
        case 'B' :
        colorCode(255,255,0);                                            // Blue Color
        break;
        case 'M' :
        colorCode(128,255, 255);                                         // Maroon Color
        break;
        case 'C' :
        colorCode(255,0,0);                                              // Cyan Color
        break;
        case 'Y' :
        colorCode(0,0,255);                                              // Yellow Color
        break;
        case 'S' :
        colorCode(192,192,192);                                          // Silver Color
         break;
         case 'G' :
         colorCode(255, 128, 255);                                       // Green Color
         break;
         case 'P' :
         colorCode(128, 255, 128);                                       // Purple Color  
         break;
         case 'N' :
         colorCode(255, 255, 128);                                       // Navy Color
         break;
         case 'T' :
         colorCode(255, 128, 128);                                       // Teal Color
         break;
         case 'O' :
         colorCode(128, 128, 255);                                       // Olive Color
         break;
         case 'F' :
         colorCode(0, 255, 0);                                           // Magenta / Fuchsia Color 
         break; 
         case 'W' :
         colorCode(0, 0, 0);                                           // white Color 
         break; 
    }
}

/***********************************************************************************************************************
 Function : Colorcode() work for common anode type leds
 Function description : Colorcode() function takes three input values for Red, Green, Blue and generates the software
 PWM for generating user required Color.  
 **********************************************************************************************************************/
void colorCode(int red, int green, int blue)
{
if(commonAnode){
    PWM0DC = 255 - red;
    PWM1DC = 255 - green;
    PWM2DC = 255 - blue;
}
else{
    PWM0DC = red;
    PWM1DC = green;
    PWM2DC = blue;
}
}

/*********************************************************************************************************************
  Function Name : brightLED()                                                                                                                                                       
  Function description : brightLED() function is used for brightness control of LED. This function generates the PWM  
  wave of 3.9 KHz frequency which is using internal CCP1 module of pic micro controller at RC2 pin for user defined   
  brightness value[duty_cycle]. In our project, the  LED is connected at RC2 pin, so that the LED will glow with user   
  defined brightness. Refer PWM. c file for  PWM1_Init() and PWM1_setDC() function definitions.                                     
**********************************************************************************************************************/
void brightLED(int dutyCycle)
{  
    if(commonAnode){
        if(dutyCycle < 100){
        dutyCycle = 100-dutyCycle;
        }
        else if(dutyCycle == 100){
            dutyCycle=99;
        }
    }
    PWM1_Init(255);                                                      // generating PWM wave with pwm period of 255, pwm frequency of 3.9 KHz 
    float ontime =((float)1023 / 100)  * dutyCycle;                      // Formula to calculate the duty_cycle from user defined brightness 
    int duty =(int)ontime;
    PWM1_setDC(duty);                                                    // calling PWM1_setDC function for generating pwm wave with user defined 
                                                                         // brightness[duty_cycle]                  
}

/***********************************************************************************************************************
 Function : fadeLed()
 Function description : fadeled() function takes four inputs which are Brightness, Period, fadeAmount,color, and
 perform LED fade action according to given fadeAmount value and with these inputs. 
 **********************************************************************************************************************/
//void ledFade(int _Brightness, double _Period, int _fadeAmount, char _color) {
//    TMR1_Init(); // Initialize timer1 for software pwm
//    unsigned int TotalTime = 0; // Initiate TotalTime = 0
//    unsigned int delay;
//    unsigned int flag;
//    int intensity = 0;
//    unsigned long int delay1 = ((_Period) * (_XTAL_FREQ / 4)) / (65535 * 2); // Formula to calculate the required count for given time period 
//    while (TotalTime < delay1) {
//        colorControl(_color);
//        if(intensity == _Brightness)flag = 0;
//        else if(intensity == 0)flag = 1;                                    // it check High/Low value of brightness value and sets flag variable
//        if(flag){
//            if(intensity + _fadeAmount <= _Brightness){                       // it prevents function to go beyond the defined Brightness value
//            intensity += _fadeAmount;
//            }
//            else intensity = _Brightness;
//        }
//        else{
//            if(intensity-_fadeAmount >= 0){                                  // it prevents function to go beyond the defined Brightness value
//            intensity -= _fadeAmount;
//            }
//            else intensity = 0;
//        }
//        brightLED(intensity);                        // Turn ON LED with user defined Color and brightness
//        delay = timePeriod(0.02, TotalTime, delay1); // Delay function for LED ON time
//        TotalTime = delay;
//    }
//    brightLED(0);
//}

void ledFade_time(unsigned int fadeInminBright,unsigned int fadeInmaxBright,unsigned int fadeOutminBright, unsigned int fadeOutmaxBright, char _color) {
    TMR1_Init(); // Initialize timer1 for software pwm
    unsigned int TotalTime = 0; // Initiate TotalTime = 0
    unsigned int delay;
    unsigned long int delay1 = ((valid_string.Totaltime) * (_XTAL_FREQ / 4)) / (65535 * 2); // Formula to calculate the required count for given time period 
    while (TotalTime < delay1) {
        colorControl(_color);
        for(unsigned int i = fadeInminBright;i<=fadeInmaxBright;i + valid_string.positive_fade_amount){
           brightLED(i);
           delay = timePeriod(0.02, TotalTime, delay1);
        }
        for(unsigned int i = fadeOutmaxBright;i >= fadeOutminBright;i - valid_string.negative_fade_amount){
          brightLED(i);
          delay = timePeriod(0.02, TotalTime, delay1);
        }
                                // Turn ON LED with user defined Color and brightness
        //delay = timePeriod(0.02, TotalTime, delay1); // Delay function for LED ON time
        TotalTime = delay;
    }
    brightLED(0);
}
//void fadeIn(unsigned int positive_feed_rate){
//    for(int i = valid_string.min_fade_brt;i<=valid_string.max_fade_brt;i + valid_string.positive_fade_amount){
//      brightLED(i);  
//    }
//}
//
//void fadeOut(unsigned int negative_feed_rate){
//    for(int i = valid_string.max_fade_brt;i>= valid_string.min_fade_brt;i - valid_string.negative_fade_amount){
//      brightLED(i);  
//    }
//}
/***********************************************************************************************************************
 Function : led_Blink()
 Function description : led_delay() function takes five inputs which are Ontime, Offtime, Brightness, Period, color, and
 perform LED on and off actions with these inputs. 
 **********************************************************************************************************************/
void ledBlink(int Brightness, double Period, int OnTime, int OffTime, char color) {
    TMR1_Init(); // Initialize timer1 for software pwm
    unsigned int TotalTime = 0; // Initiate TotalTime = 0
    unsigned int delay;
    unsigned long int delay1 = ((Period) * (_XTAL_FREQ / 4)) / (65535 * 2); // Formula to calculate the required count for given time period 
    while (TotalTime < delay1) {
        colorControl(color);
        brightLED(Brightness); // Turn ON LED with user defined Color and brightness
        delay = timePeriod(OnTime, TotalTime, delay1); // Delay function for LED ON time
        TotalTime = delay;
        if (TotalTime < delay1) {
            brightLED(0); // Turn OFF LED 
            delay = timePeriod(OffTime, TotalTime, delay1); // Delay function for LED OFF time
            TotalTime = delay;
        }
    }
    brightLED(0);
}

/***************************************************************************************************************************
 Function : Timer_delay()
 Function description : Timer_delay() function provides OnTime delay for LEDon() function.This function takes 'Period' 
 as an input argument and calculates the required count for generating the required delay using timer1 module. 
 In this case there are 38 counts for 1 sec delay. 
 ***************************************************************************************************************************/
void timerDelay(double Period)
{      
    int count = 0;                                        				// temporary variable to count the number of iterations of the code for given delay
    /* Formula to calculate the required count for given time period where Total_time is the user defined time,
     _XTAL_FREQ is external oscillator frequency of 20 MHz, 65535 is maximum value of 16 bit timer1 module*/
    unsigned int res = ((Period) * (_XTAL_FREQ/4)) / (65535 * 2);            
                                                                        // Enable 16-bit TMR0 register,
    T0CON=0x80;                                                         // No pre-scale, internal clock,timer on  
    TMR0 =0;                                                            // Load TMR0 to 0
    do
    {
        T0CONbits.TMR0ON=1;                                             // Turn ON Timer0 
        while(INTCONbits.TMR0IF==0);                                    // Wait for Timer0 overflow interrupt flag 
        TMR0ON=0;                                                       // Turn OFF timer 
        TMR0IF=0;                                                       // Make Timer0 overflow flag to '0' 
        count++;
		}while(count < res);                                            // the variable 'res' has total required count for given delay
                                                                        // this 'res' value is compared with count value for each iteration
                                                                        // the loop exit when total delay is elapsed i.e res = count
 }

/******************************************************************************************************************
 Function : timePeriod()
 Function description : Period_delay1() function provides OnTime and OffTime delay for LEDblink() function.This 
 function takes 'Period', 'Total_time' and 'delay1' as an input argument and calculates the required count for 
 generating the required delay using timer1 module. In this case there are 38 counts for 1 sec delay. 
 ******************************************************************************************************************/
unsigned int timePeriod(double Period, unsigned int TotalTime, unsigned long int delay1) {
    unsigned int count = 0;
    unsigned int delay;
    /* Formula to calculate the required count for given time period where Total_time is the user defined time,
     _XTAL_FREQ is external oscillator frequency of 20 MHz, 65535 is maximum value of 16 bit timer0 module*/
    unsigned int res = ((Period) * (_XTAL_FREQ / 4)) / (65535 * 2);
    // Enable 16-bit TMR0 register,
    T0CON = 0x80; // No pre-scale, internal clock,timer ON  
    TMR0 = 0; // Load TMR0 with 0
    do {
        T0CONbits.TMR0ON = 1; // Turn ON Timer0 
        while (INTCONbits.TMR0IF == 0); // Wait for Timer0 overflow interrupt flag 
        TMR0ON = 0; // Turn OFF timer 
        TMR0IF = 0; // Make Timer0 overflow flag to '0' 
        count++; // increment the count
        delay = TotalTime + count;
    } while (count < res && delay1 > delay); // condition (count< res) checks the count for ontime and offtime delay
    return delay; // condition (delay1 > delay) checks the total_time count for complete blink operation

}