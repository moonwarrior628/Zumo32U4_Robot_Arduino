/* Student Names: M. S. and T. R. 
 MicroProcessor Assignment : This assignmnet enbales the user to select items from a menu and perform various functions on the Zumo32U4 robot. 
 It records acceleration in x, y and z coordinates using the LSM303 function and moves the robot forward in both Right and Left direction using motor function. 
 It also records data from the encoders using encoders function and has a buzzer and an LED function.

==This project was influenced or inspired by Arduino Library examples such as BuzzerBasics, BlinkLEDs, MotorTest, Encoders and InertialSensors. */

//Inlcudes Libraries 
#include <Wire.h> // for comapss function 
#include <Zumo32U4.h> // all robot commands

// These objects provide access to the Zumo 32U4's
// on-board buttons.
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;
LSM303 compass;
Zumo32U4Buzzer buzzer;

// Declaring menu integer to be used in switch case statements.
int menu=1;

// A function that only runs once before all other functions.
void setup()
 {
    // Initializes communication with the compass function 
    Wire.begin();
    if (!compass.init()) // if statement that alerts the user when compass is not initialized 
      {
        // When Red LED is on it means the compass is not detected by the robot.
        ledRed(1);
      }
    //Enables deafult settings for compass
    compass.enableDefault();
     
     lcd.clear(); //clears screen
     lcd.print("Demo"); // prints message on screen
     lcd.gotoXY(0,1);
     lcd.print("Press B");
     
     buttonB.waitForButton(); // waits for the user to press button B 
  
     lcd.clear();
 }

// Loop function that contains the switch statements for the menu.
void loop()
 {
    lcd.clear();
    switch (menu)

  {
    //This case is responsible for turning on the Right Motor 
      case 1: 
  
         lcd.print("Rmotor");
         
           if (buttonB.isPressed())
            {
                // Whenever the button B is pressed, turn on the Right Motor
                int speed=200; // Initialize speed of the motor
                motors.setRightSpeed(speed); //setting speed of the right motor
                delay(2);// adds a delay of 2 milliseconds
            }
            else
            {
                // Whenever the button is not pressed, turn off the Right Motor
                motors.setRightSpeed(0);
                delay(2);
            }
          
            //The getSingleDebouncedPress function only records one value and discards other signals from the buttons.
            if(buttonC.getSingleDebouncedPress()) 
            {
               menu=menu+1; //When button c is pressed the next item on menu is displayed 
            }
    
            if(buttonA.getSingleDebouncedPress())
            {
               menu=9; // when button A is pressed the item 9 is displayed.
            }
            
          break;
          
  // This case is responsible for turning on the Left Motor   
    case 2: 
  
        lcd.print("Lmotor");
  
          if (buttonB.isPressed())
            {
                int speed=200;
                motors.setLeftSpeed(speed);
                delay(2);
            }
          else
            {
                motors.setLeftSpeed(0);
                delay(2);
            }
  
          
          if(buttonC.getSingleDebouncedPress())
            {
                menu=menu+1;
            }
  
          if(buttonA.getSingleDebouncedPress())
            {
                menu=menu-1;
            }
          
          break;

// This case is responsible for recording how many times the Right Motor moves.
    case 3:
  
       lcd.print("REncoder");
       lcd.gotoXY(0,1);
       lcd.print(encoders.getCountsRight()); //gets record from encoders function and prints on the screen.
    
          if(buttonC.getSingleDebouncedPress())
            {
               menu=menu+1;
            }
        
            if(buttonA.getSingleDebouncedPress())
            {
               menu=menu-1;
            }
        
     break;
     
 // This case is reponsible to record data from the Left encoder.    
    case 4:
  
    lcd.print("LEncoder");
    lcd.gotoXY(0,1); 
    lcd.print(encoders.getCountsLeft());
  
         if(buttonC.getSingleDebouncedPress())
           {
               menu=menu+1;
           }
        
           if(buttonA.getSingleDebouncedPress())
           {
              menu=menu-1;
           }
      
     break;
  
 // This case is responsible to record raw value of acceleration in the x coordinate.     
    case 5:
  
     lcd.print("X-acc");
     compass.read(); // Reads data from the compass 
     lcd.print(compass.a.x);// Prints raw value of accelerometer (of X coordinate)
      
         if(buttonC.getSingleDebouncedPress())
           {
              menu=menu+1;
           }
            
           if(buttonA.getSingleDebouncedPress())
           {
              menu=menu-1;
           }
        
      break;

  // This case is responsible to record raw value of acceleration in the y coordinate.  
     case 6:
     
      lcd.print("Y-acc"); 
      compass.read();
      lcd.print(compass.a.y);
      
        if(buttonC.getSingleDebouncedPress())
          {
             menu=menu+1;
          }
  
          if(buttonA.getSingleDebouncedPress())
          {
             menu=menu-1;
          }
  
     break;
     
   // This case is responsible to record raw value of acceleration in the z coordinate.    
    case 7:
  
      lcd.print("Z-acc");      
      compass.read();       
      lcd.print(compass.a.z);
    
        if(buttonC.getSingleDebouncedPress())
          {
             menu=menu+1;
          }
      
         if(buttonA.getSingleDebouncedPress())
          {
             menu=menu-1;
          }
    break;

 // This case is responsible to play the Buzzer when Button B is pressed. 
    case 8:
  
      lcd.print("Buzzer");
  
        if (buttonB.isPressed())
         {
            buzzer.playFrequency(440, 200, 15);
         }

        if(buttonC.getSingleDebouncedPress())
         {
            menu=menu+1;
         }
  
         if(buttonA.getSingleDebouncedPress())
         {
            menu=menu-1;
         }
  
      break;

  //This case is responsible to switch on the Yellow LED when the Button B is pressed.
    case 9:
  
     lcd.print("YellowLED");
  
       if (buttonB.isPressed())
        {
           ledYellow(1);
        } 
       else  
        {
           ledYellow(0); // The LED switches off if Button B is nto pushed.
        }
  
       if(buttonC.getSingleDebouncedPress())
       {
           menu=1;
       }
  
       if(buttonA.getSingleDebouncedPress())
       {
           menu=menu-1;
       }
  
    break;
       
   default: 
   menu = 1;
  
   break; 
    }
    
   Serial.print(menu);
   delay(50);
}
