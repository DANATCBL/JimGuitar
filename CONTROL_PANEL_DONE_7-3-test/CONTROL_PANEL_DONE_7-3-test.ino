//CONTROL_PANEL_DONE_6_13_16_WC
//alarm code stripped down


//################################################################################
//########################### SECTION 100 - INCLUDES
//#################################################################################



 
// constants won't change. Used here to assign names to led pins. Assigning names to 
//pin numbers is not necessary, it just makes it easier to 
// refer to the pins later on in the program. Status of pins as OUTPUT pins will be 
//done in SETUP section



#include "ST7565.h"              // FILE NECESSARY FOR RUNNING THE FLAT PANEL DISPLAY

// *********************** ASSIGN LED PINS AND ASSOCIATED "STATE" VARIABLES *********

//#################################################################################
//########################### SECTION 110 - PANEL GREEN LEDS
//#################################################################################
                                  //  PIN FUNCTIONS ASSIGNED IN SECTION 200
// **** AIR PRESSURE STATUS LED
const byte airPressG = 13;        // GREEN LED PIN


// **** WATER PUMP STATUS LED
const byte waterPumpG =  46;      // GREEN LED PIN 46


// **** WATER PRESSURE STATUS LED
const byte waterPressG =  44;     // GREEN LED PIN 


// **** VACUUM STATUS LED
const byte vacG =  9;             // GREEN LED PIN 


// **** LASER POWER SUPPLY STATUS LED
const byte laserPowerG = 7;       // GREEN LED PIN 


// **** CNC POWER SUPPLY STATUS LED
const byte cncPowerG =  45;       // GREEN LED PIN 


// **** DOOR STATUS LED 
const byte doorG =  4;           // GREEN LED PIN 



//#################################################################################
//########################### SECTION 120 - LED DISPLAY PIN ASSIGNMENTS
//#################################################################################
                                       
// **** DISPLAY BACKLIGHTs ****         // FUNCTION ASSIGNMENTS IN SECTION 210
const byte redBg = 8;                    // 0 IS BRIGHTEST, 255 DARKEST
const byte greenBg = 6;
const byte blueBg = 3;

// **** DISPLAY PIN ASSIGNMENTS -------- COMPLETE DIAGRAM OF DISPLAY WIRING AT 

// ST7565 glcd(9, 8, 7, 6, 5);  ** ORIGINAL PIN ASSIGNMENTS, as used in Adafruit instructions
ST7565 glcd(50, 51, 52, 14, 15);   // Arduino pins that the 4050 connects to directly.
                                   // 4050 pin 14 ----> Arduino 50 (Brown)
                                   // 4050 pin 11 ----> Arduino 51 (Orange)
                                   // 4050 pin 7  ----> Arduino 52 (Yellow)
                                   // 4050 pin 5  ----. Arduino 14 (Green)
                                   // 4050 pin 3  ----> Arduino 15 (Red)
                                   
// **** LCD DISPLAY TIMED FLASH ASSIGNMENTS        //USED IN SECTION 320

byte flashToggle = 0;
int long currentfMillis;
int long oldfMillis;
unsigned long flashMillis = 0;

byte flipper1 = 100;
byte flipper2 = 0;
byte bright;

//String message = "";

// ******************  DISPLAY LOGIC TOGGLES *******************

int manMsgToggle = 0;
int fireMsgToggle = 0;
int warnMsgToggle = 0;






//#################################################################################
//########################### SECTION 130 - ASSORTED ASSIGNMENTS
//#################################################################################
/*
// NOT USED YET IN ANY CODE
// **** BOB PIN 6 HOT DETECTION    WHEN BOB PIN 6 IS GROUNDED, LASER FIRES!!!!!!!!!






// **** BOB PIN 6 HOT DETECTION    WHEN BOB PIN 6 IS GROUNDED, LASER FIRES!!!!!!!!!
const int bob6Hot = 26;      // USED IN ALARM LOGIC, SECTION ___________
*/

//#################################################################################
//########################### SECTION 135 - ALARM FUNCTION ASSIGNMENTS
//#################################################################################

//                        ALARM LOGIC IN SECTION 320 

// **** BUZZER PIN
byte alrmTimerToggle = 0;                          // TOGGLE VARIABLE US USED TO START AND STOP BUZZER
byte buzzerState;                            //VARIABLE TO HOLD STATE OF BUZZER UNTIL IT IS GIVEN TO buzzer
const int buzzer = A2;                      // ANALOG PIN FOR BUZZER HOT LEAD

// **** UTILITY BUTTON
const byte utilityButton = 43;      //INPUT PIN FOR PANEL BUTTON, USED TO SILENCE ALARMS
byte buzzerToggle = 0;              // CONSIDERED "UTILITY" BUTTON BECAUSE IT CAN BE 
byte utilityButtonState = 0;        //PROGRAMMED FOR OTHER USES BESIDES ALARMS

// SETS TIME INTERVAL THAT ALARM WILL WAIT BEFORE RESETTING. 
unsigned long previousalrmMillis  = 0;           // 
unsigned long alrmMillis;
unsigned long alrmInterval = 10000;             //1 MINUTE INTERVAL


//#################################################################################
//########################### SECTION 140 - SENSOR PIN NAMES
//#################################################################################
                                     // SENSOR PIN ASSIGNMENTS IN SECTION 220

// **** SENSOR INPUT PINS
const byte lPower = 25;
const byte cncOn = 22;
const byte waterOn = 24;
const byte waterpressOn = 18;
const byte airOn = 19;
const byte doorOpen = 23;

// **** VACUUM DETECT CIRCUIT
byte vacOn = 0;     
byte photocellReading;
int photocellPin = A0;


//#################################################################################
//########################### SECTION 150 - MANAUL AND TIMED LASER FIRING CIRCUITS PIN ASSIGNMENTS 
//#################################################################################
                                         // MANUAL AND TIMED FIRING LOGIC IN SECTION 310
                                         

const byte manFireLed = 42;      // PANEL LIGHT, ACTIVATES WHEN IN MANUAL FIRE MODE
                                // USED IN SECTION 310
                                         

// **** ROTARY SELECTOR

int rotarySetting;

const byte rotary1 = 28;
const byte rotary2 = 48;
const byte rotary3 = 34;
const byte rotary4 = 35;
const byte rotary5 = 33;



// **** MANUAL & TIMED FIRE CONTROL VARIABLES

const byte manFirePin = 17;      //input pin that detects when the manual fire pin is depressed.
long laserInterval;             // holds time interval during timed firing
int fireTime;
unsigned long previousLaserMillis = 0;
unsigned long currentLaserMillis;


// **** TIMED FIRE CONTROL VARIABLES
const byte timedRelay = 20;      
byte buttonToggle = 0;           // used to set the state of the laser during timed firing.

const byte circuitRelay = 30;      // ACTIVATES CIRCUIT CONTROL RELAY, DEACTIVATING AUTO FIRING CIRCUIT AS A 
                              // SAFETY MEASURE WHEN MANUALLY FIRING.
byte circuitRelayState;

//#################################################################################
//########################### SECTION 160 GENERAL PURPOSE TIMER
//#################################################################################

// **** 

unsigned long tsMillis;
unsigned long currenttsMillis;
unsigned long tsInterval = 1000;             //TWO SECOND INTERVAL




//###############################################################################
//##########################  TIMERS USED IN THIS CODE 
//###############################################################################
/*

          f               LCD FLASH (ALARM)                   SECTION 120
          alrm            ALARM BUZZER RESET                  SECTION 135
          Laser           TIMED LASER FIRING                  SECTION 150
          ts              TROUBLESHOOTING/GENERAL PURPOSE     SECTION 160

*/
void setup() // ******************************** SETUP STARTS HERE *********************
{
Serial.begin(9600);

//#################################################################################
//########################### SECTION 200 - CONTROL PANEL LED ASSIGNMENTS
//#################################################################################
                                           // PIN NUMBERS ASSIGNED IN SECTION 110
                                        
  
// **** AIR PRESSURE STATUS LED            
pinMode (airPressG, OUTPUT);                        

// **** WATER PUMP STATUS LEDs
pinMode (waterPumpG, OUTPUT);      
      
// **** WATER PRESSURE STATUS LEDs
pinMode (waterPressG, OUTPUT);

// **** VACUUM STATUS LEDs
pinMode (vacG, OUTPUT); 

// **** LASER POWER SUPPLY STATUS LEDs
pinMode (laserPowerG, OUTPUT);  

// **** CNC POWER SUPPLY STATUS LEDs
pinMode (cncPowerG, OUTPUT);     
   
// **** DOOR STATUS LEDs 
pinMode (doorG, OUTPUT);


//#################################################################################
//########################### SECTION 220 - CONTROL PANEL PIN FUNCTION ASSIGNMENTS
//#################################################################################
                                           // PIN NUMBER ASSIGNMENTS IN SECTION 140
                                 

// **** TROUBLESHOOTING PINS

pinMode (utilityButton, INPUT);          // RED ALARM SHUT-OFF BUTTON ON CONTROL PANEL


// ********* ASSIGN NAMED PINS WITH "INPUT" FUNCTION IN ORDER TO ACT AS SWITCH-STATE SENSORS
                                                   // SENSOR LOGIC IN SECTION 300
pinMode (lPower, INPUT);
pinMode (cncOn, INPUT); 
pinMode (waterOn, INPUT);
pinMode (waterpressOn, INPUT);
pinMode (airOn, INPUT);
pinMode (vacOn, INPUT);
pinMode (doorOpen, INPUT);
pinMode (manFirePin, INPUT);            //input pin to detect when manual fire pin is pressed

//pinMode (bob6Hot, INPUT);               // DETECTS WHEN THE BREAK OUT BOARD ID FIRING THE LASER
 


// **** MANUAL LASER RELAY
pinMode (circuitRelay, OUTPUT);         //controls the state of the circuit control relay during timed fire.
pinMode (timedRelay, OUTPUT);           // controls the state of the timed fire relay

pinMode (manFireLed, OUTPUT);
pinMode (buzzer, OUTPUT); 


//#################################################################################
//########################### SECTION 210 - LED DISPLAY BACKLIGHT PIN ASSIGNMENTS
//#################################################################################
                                           // PIN ASSIGNMENTS IN SECTION 120

// **** DISPLAY BACKLIGHT LEDs
pinMode (redBg, OUTPUT);
pinMode (greenBg, OUTPUT);
pinMode (blueBg, OUTPUT);


//#################################################################################
//########################### SECTION 230 - LED DISPLAY INITIAL VALUES
//#################################################################################
 

 analogWrite(greenBg, 100);              // STARTS THE DISPLAY AND PRINT INITIAL MESSAGE
 analogWrite(blueBg, 255);
 analogWrite(redBg, 0);
 glcd.begin(0x25);                     // initialize and set the character contrast to 0x18
  
 glcd.display();                       // show splashscreen 
 //glcd.clear();

                      
pinMode(rotary1, INPUT);
pinMode(rotary2, INPUT);
pinMode(rotary3, INPUT);
pinMode(rotary4, INPUT);
pinMode(rotary5, INPUT);

flashMillis = millis(); 
tsMillis = millis();
}  // ************************** END OF SETUP ****************************************




void loop()  // ************************** LOOP STARTS HERE ***************************
{

//#################################################################################
//########################### SECTION 300 - CONTROL PANEL SENSOR LOGIC
//#################################################################################
                                        // PIN ASSIGNMENTS IN SECTION 220
  
  //SENSOR CIRCUITS ARE "PULL DOWN", THE INPUT PIN DETECTS A "LOW" STATE UNTIL THE 
  //SWITCH IS CLOSED, AND THE PIN THEN DETECTS A "HIGH" STATE. THE PIN VALUE IS SAVED
 // IN A "STATE" VARIABLE FOR LATER WRITING TO THE ACTUAL PIN
     
     // ********* DOOR OPEN *********************


   if (digitalRead(doorOpen) == HIGH)
     {
        analogWrite(doorG, 0);
     }
    else 
     {
       analogWrite(doorG, 255);
     }
    
     // ********* LASER POWER ******************
     
   if (digitalRead(lPower) == HIGH)
     {
       analogWrite(laserPowerG, 0);
     }
    else 
     {
       analogWrite(laserPowerG, 255);
     }
         
     // ********* CNC POWER SUPPLY **************
     
    if (digitalRead(cncOn) == HIGH)
      {
        analogWrite(cncPowerG, 0);
      }
    else 
      {
        analogWrite(cncPowerG, 255);
      }
      
     // ********* LASER FIRING ************
 /*    
    if (digitalRead(lPower) == HIGH)
      {
        analogWrite(laserPowerG, 0);
      }
    else 
      {
        analogWrite(llaserPowerG, 255);
      }

  */ 
     // ********* AIR PRESSURE ******************
     
    if (digitalRead(airOn) == HIGH)
      {
        analogWrite(airPressG, 0);
      }
    else 
      {
        analogWrite(airPressG, 255);
      }
    
     // ********* WATER PUMP ********************
     
    if (digitalRead(waterOn) == HIGH)
      {
        analogWrite(waterPumpG, 0);
      }
    else 
      {
        analogWrite(waterPumpG, 255);
      }
     
      
    // ********** WATER PRESSURE ****************
    
    if (digitalRead(waterpressOn) == HIGH)
      {
        analogWrite(waterPressG, 0);
      }
    else 
      {
        analogWrite(waterPressG, 255);
      }
  
    // ********* VACUUM DETECTION ************************
    
    photocellReading = analogRead(photocellPin);    
    // VACUUM DETECTOR USES ANALOG INPUT, EXPRESSED AS A NUMERICAL VALUE

    if (photocellReading < 250)                     
    // ADJUST THIS NUMERICAL READING TO MATCH LIGHT LEVELS INSIDE CASE
      {
        analogWrite(vacG, 0);
      }
    else 
      {
        analogWrite(vacG, 255);
      }
  
//#################################################################################
//########################### SECTION 310 - LASER FIRING CIRCUITS LOGIC
//#################################################################################  
                                       // PIN ASSIGNMENTS IN SECTION 150

 

// *************** MANUAL FIRING CIRCUIT ACTIVATION LOGIC ******************

 rotarySetting = timerInterval();               // reads the rotary setting and sets the timer interval

 if (rotarySetting > 10)                       // if manual/timed fire setting is selected, light the "ON" led
    {
      manMsgToggle = 1;
      
/*      
      analogWrite(greenBg, 200);
      analogWrite(blueBg, 255);
      analogWrite(redBg, 0);
      glcd.clear();
      glcd.drawstring(0, 3, "   MANUAL/TIMED FIRE");
      glcd.display();
*/      
      digitalWrite(manFireLed, HIGH);
   
      circuitRelayState = HIGH;                 // if manual/timed fire selected, disable the BOB firing circuit
    }
 else
    { 

       digitalWrite(manFireLed, LOW);
       circuitRelayState = LOW;
     
     //  if (alrmTimerToggle == 0)   //^^^^^^^^^^^^^^
     //    { 
          manMsgToggle = 0;
      //   }
     } 

 if (digitalRead(manFirePin) == HIGH && rotarySetting > 10)      //if fire button pressed and in manual/timed fire mode...
     {
  
       if (buttonToggle == 0)                         // checks if button is not already pressed  
         { 
           buttonToggle = 1;                          //set toggle to 1. this prevents this code block from running again until toggle is set back to 0 when laser firing interval is reached.
                     //enables the manual firing circuit. This fires the laser.    
           previousLaserMillis = millis();             //gets the current time in milliseconds. This time becomes the "start" time when using the timed firing functionality. The current time will be compared to this time to determin how much time has passed.
        
         }        
     }

 if (buttonToggle == 1)
 { 
   digitalWrite(timedRelay, HIGH);
   if (millis() - previousLaserMillis >= rotarySetting)  //checks the time interval since currentLaserMillis was set previously. If time is reached, code shuts of laser, restores auto firing circuit and resets buttonToggle      
       { 
         circuitRelayState = LOW;                      //restores auto firing circuit.                           
         digitalWrite(timedRelay, LOW);       
         if (digitalRead(manFirePin) == LOW)          // prevents repeated timed fire if button stays depressed; wont reset buttonToggle to 0 until button is releasee
            {
              buttonToggle = 0;                         // resets the toggle to 0, allowing the startup function to run the next time the manual fire button is pressed.
            }
       }           
    else if (laserInterval == 10000)                 // if rotary setting indicates manual fire, only stop firing when button is released.  
       {
         if( digitalRead(manFirePin) == LOW)         // releasing the manual fire button
            {  
              digitalWrite(timedRelay, LOW);          //turn off timed fire relay
              circuitRelayState = LOW;                //deactivates the circuit control relay, returning control of laser back to BOB (auto mode)
              buttonToggle = 0;                       // resets the toggle to 0, allowing the startup function to run the next time the manual fire button is pressed.
          
            }
        }
 } 
  // 
  
 currenttsMillis = millis();
   if(currenttsMillis - tsMillis >= tsInterval) 
   {
     
   } 
   
//#################################################################################
//########################### SECTION 320 - ALARMS LOGIC
//#################################################################################


 if (alrmTimerToggle == 0)
  {
   if (rotarySetting == 10 && digitalRead(lPower) == HIGH)                                       // if set to "auto" and laser PS is on AND either the CNC PS OR air pressure OR water pressure OR vacuum is off, 
    {
     if (digitalRead(cncOn) == LOW || digitalRead(airOn) == LOW || digitalRead(waterpressOn) == LOW || photocellReading < 250)   // then trigger alarm. this enforces the laser PS "last on/first off" policy. 
      {
        alarm(0);                  //CALLS THE alarm() FUNCTION (SECTION 400), WHICH SETS THE ALARM STATE TO "ON"
        oldfMillis = millis();
        flashToggle = 1;   //TOGGLES ON THE LCD FLASHING BACKLIGHT                 
        //   ***** lcd message *********
        warnMsgToggle = 1;
   /*     
        glcd.clear();       // clears display
        glcd.drawstring(0, 3, "  CHECK LASER          POWER SUPPLY!");  //writes text to buffer
        glcd.display();   //sends buffer contents to display
        previousalrmMillis = millis();         
*/
      }
      else
      {warnMsgToggle = 0;}
    }
  }
  
 if (flashToggle == 1)                             // THIS CODE BLOCK SWITCHES LCD BACKLIGHT BETWEEN TWO COLORS AT 500 MILLISECOND INTERVALS AS LONG AS FLASHTOGGLE IS SET TO 1
   {
      currentfMillis = millis();
      analogWrite(greenBg, 255);
      analogWrite(blueBg, 255); 
      if (currentfMillis - oldfMillis >= 500)
      {
         bright = flipper1;
         analogWrite(redBg, bright);
         flipper1 = flipper2;
         flipper2 = bright;
         oldfMillis = currentfMillis;
      }
   
   }

 if (alrmTimerToggle == 1)   
    {
      alrmMillis = millis();
      if (alrmMillis - previousalrmMillis >= alrmInterval)     //RESETS THE ALARM AFTER A SET INTERVAL.
      {
         
//        dspMsg();
        alrmTimerToggle = 0;
        warnMsgToggle = 0;


      }
      if (digitalRead(lPower) == LOW)
      { 
        flashToggle = 0;
        buzzerState = 0;
        alrmTimerToggle = 0;
        warnMsgToggle = 0;
      }
    }
 digitalWrite(circuitRelay, circuitRelayState);

 analogWrite(buzzer, buzzerState);
    
 if (digitalRead(utilityButton) == HIGH)  
  {
   buzzerState = 0;
  // flashToggle = 0;
 //  dspmsg();
   } 
    
    
    
 if (manMsgToggle == 1)
 { manualMsg();}
 if (warnMsgToggle == 1)
 {warnMsg();}
 if (manMsgToggle == 0 && warnMsgToggle == 0)
 {dspMsg();}
    
    

}




int timerInterval()
{if (digitalRead(rotary4) == HIGH)
 {laserInterval = 10000;}
 
 else if (digitalRead(rotary3) == HIGH)
 {laserInterval = 250;}
 
 else if (digitalRead(rotary5) == HIGH)
 {laserInterval = 500;}
 
 else if (digitalRead(rotary1) == HIGH)
 {laserInterval = 1000;}
 
 else if (digitalRead(rotary2) == HIGH)
 {laserInterval = 10;}
 
 
 return laserInterval;
 
 if (manMsgToggle == 1)
 { manualMsg();}
 if (warnMsgToggle == 1)
 {warnMsg();}
 if (manMsgToggle == 0 && warnMsgToggle == 0)
 {dspMsg();}
 
 
 
 
 
 
 
 
}




void dspMsg()        //function to display the default-state message
{     
 analogWrite(greenBg, 100);            
 analogWrite(blueBg, 255);
 analogWrite(redBg, 0);
 glcd.clear();
 glcd.drawstring(0, 2, "  BLASTERBOT 5000                          ALL SYSTEMS NORMAL     ");
 glcd.display();
}
void manualMsg()        //function to display the default-state message
{     
analogWrite(greenBg, 200);
      analogWrite(blueBg, 255);
      analogWrite(redBg, 0);
      glcd.clear();
      glcd.drawstring(0, 3, "   MANUAL/TIMED FIRE");
      glcd.display();
}

void warnMsg()        //function to display the default-state message
{     
        glcd.clear();       // clears display
        glcd.drawstring(0, 3, "  CHECK LASER          POWER SUPPLY!");  //writes text to buffer
        glcd.display();   //sends buffer contents to display
        previousalrmMillis = millis();
}

void alarm(int type)
{ 
  if (type == 0)         //TYPE 0 ALARM, AFTER BEING TRIGGERED, WILL NOT ACTIVATE AGAIN FOR 60 SECONDS
  {buzzerState = 255;
   alrmTimerToggle = 1;
  
 }
}


       
