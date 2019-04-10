
/*This program calibrates the distance sensed by the proximity sensors and goes towards objects and pushes it 
 * off the board while maintaining itself inside the black bordered board. This code will run only for 3 
 * minutes as per the requirements. Initially proximity sensors are used to sense the distance between objects 
 * and line sensors make sure the robot stay inisde the black border at all times during the run. The black line 
 * is detected by keeping a threshold value from 290 or greater. The if statments account for all situations 
 * during the run as it orders the robot to reverse and turn respective direction when avoiding the black line.
 * Credits given to: Arduino Library (Zumo32U4)*/

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
uint16_t lineSensorValues[5];


bool proxLeftActive;
bool proxFrontActive;
bool proxRightActive;
#define NUM_SENSORS 5
//uint16_t lineSensorValues[5];
unsigned long starttime;
//unsigned long starttime2;
void setup()
{
  proxSensors.initThreeSensors();
  loadCustomCharacters();
  lineSensors.initFiveSensors();
  lineSensors.read(lineSensorValues);
  if (lineSensorValues[0] >=300) 
  {
       motors.setRightSpeed(0);
       motors.setLeftSpeed(85);
       delay(1000);    
  }
  else if (lineSensorValues[4] >=300)

  {
      motors.setRightSpeed(85);
      motors.setLeftSpeed(0);
      delay(1000);   
  }
  
  buttonA.waitForButton();
  
  if (buttonA.isPressed())
    {
      starttime = millis();
    }
  
}

// This function loads characters that are in the form of bars displayed on the screen
void loadCustomCharacters()
{
  static const char levels[] PROGMEM = {
    0, 0, 0, 0, 0, 0, 0, 63, 63, 63, 63, 63, 63, 63
  };
  lcd.loadCustomCharacter(levels + 0, 0);  // 1 bar
  lcd.loadCustomCharacter(levels + 1, 1);  // 2 bars
  lcd.loadCustomCharacter(levels + 2, 2);  // 3 bars
  lcd.loadCustomCharacter(levels + 3, 3);  // 4 bars
  lcd.loadCustomCharacter(levels + 4, 4);  // 5 bars
  lcd.loadCustomCharacter(levels + 5, 5);  // 6 bars
  lcd.loadCustomCharacter(levels + 6, 6);  // 7 bars
}
// This function prints the bars on the lcd screen
void printBar(uint8_t height)
{
  if (height > 8)
  {
    height = 8;
  }
  const char barChars[] = {' ', 0, 1, 2, 3, 4, 5, 6, 255};
  //lcd.print(barChars[height]);
}

// Prints a bar graph showing all the sensor readings on the LCD.
void printReadingsToLCD()
{
  // On the first line of the LCD, display proximity sensor
  // readings.
  lcd.gotoXY(0, 0);
  printBar(proxSensors.countsLeftWithLeftLeds());
  printBar(proxSensors.countsLeftWithRightLeds());
  //lcd.print(' ');
  printBar(proxSensors.countsFrontWithLeftLeds());
  printBar(proxSensors.countsFrontWithRightLeds());
  //lcd.print(' ');
  printBar(proxSensors.countsRightWithLeftLeds());
  printBar(proxSensors.countsRightWithRightLeds());

  // On the second line of the LCD, display line sensor readings.
  // These calibrated sensor readings are between 0 and 1000, so
  // we use the map function to rescale it to 0 through 8.
  lcd.gotoXY(0, 1);

  // On the last 3 characters of the second line, display basic
  // readings of the sensors taken without sending IR pulses.
  lcd.gotoXY(5, 1);
  printBar(proxLeftActive);
  printBar(proxFrontActive);
  printBar(proxRightActive);
}

// Prints a line with all the sensor readings to the serial
// monitor.
void printReadingsToSerial()
{
  static char buffer[80];
  sprintf(buffer, "%d %d %d %d %d %d  %d %d %d  %4d %4d %4d %4d %4d\n",
          proxSensors.countsLeftWithLeftLeds(),
          proxSensors.countsLeftWithRightLeds(),
          proxSensors.countsFrontWithLeftLeds(),
          proxSensors.countsFrontWithRightLeds(),
          proxSensors.countsRightWithLeftLeds(),
          proxSensors.countsRightWithRightLeds(),
          proxLeftActive,
          proxFrontActive,
          proxRightActive);
  Serial.print(buffer);
}

void loop()
{
 if ((millis() - starttime)< 180000)
 {
  static uint16_t lastSampleTime = 0;
  int distanceininchesL;
  int distanceininchesR;
  int x;
  //lineSensors.read(lineSensorValues[0],lineSensorValues[4]);
  x = proxSensors.countsFrontWithLeftLeds() - proxSensors.countsFrontWithRightLeds();


  if ((uint16_t)(millis() - lastSampleTime) >= 100)
  {
    
    //endtime = starttime;
    
    lastSampleTime = millis();

    // Send IR pulses and read the proximity sensors.
    proxSensors.read();

    // Just read the proximity sensors without sending pulses.
    proxLeftActive = proxSensors.readBasicLeft();
    proxFrontActive = proxSensors.readBasicFront();
    proxRightActive = proxSensors.readBasicRight();
    lineSensors.read(lineSensorValues);

    // These functions print the distances corresponding to the bar height that determine the distances sensed by the sensors.Each bar has its own value in inches.
    lcd.clear(); // clears screen to print other values (bar height )
    delay(100);
    lcd.gotoXY(0, 0);

    if ((proxSensors.countsFrontWithLeftLeds()  == 0)) lcd.print("L55.560");
    if ((proxSensors.countsFrontWithLeftLeds()  == 1)) lcd.print("L47.320");
    if ((proxSensors.countsFrontWithLeftLeds()  == 2)) lcd.print("L39.084");
    if ((proxSensors.countsFrontWithLeftLeds()  == 3)) lcd.print("L30.840");
    if ((proxSensors.countsFrontWithLeftLeds()  == 4)) lcd.print("L22.610");
    if ((proxSensors.countsFrontWithLeftLeds()  == 5)) lcd.print("L14.370");
    if ((proxSensors.countsFrontWithLeftLeds()  == 6)) lcd.print("L6.135");

    lcd.gotoXY(0, 1);
    if ((proxSensors.countsFrontWithRightLeds()  == 0)) lcd.print("R55.560");
    if ((proxSensors.countsFrontWithRightLeds()  == 1)) lcd.print("R47.320");
    if ((proxSensors.countsFrontWithRightLeds()  == 2)) lcd.print("R39.084");
    if ((proxSensors.countsFrontWithRightLeds()  == 3)) lcd.print("R30.840");
    if ((proxSensors.countsFrontWithRightLeds()  == 4)) lcd.print("R22.610");
    if ((proxSensors.countsFrontWithRightLeds()  == 5)) lcd.print("R14.370");
    if ((proxSensors.countsFrontWithRightLeds()  == 6)) lcd.print("R6.135");
  }
 

  /* AVOID MODE: This part of program detects obstacles within 0.375-16 inches(it cannot obey the 7.8 inches required
     by the assignment as we cannot obtain a value between bar 6 and bar 5(0.375-16inches).
     It detects the distance away from both from left and right front sensors and turns accordingly. The if
     statements calculate the distance away from each sensors (Left and Right) and go towards the obstacle. The
     'x' variable is the difference between distances from each sensors, if the distance is less than 0(if the obstacle
     is much away from the front left sensor while it being closer to the front right sensor) the robot goes more 
     towards the object. When the difference is greater than zero the robot turns right and when the difference is 
     equal to zero it turns right goes back again on  the if cases to go towards the obstacles.
     The else statement is responsible for moving the robot straight assuming there is a obstacle in its way. */
 int rightSpeed = 0;
  int leftSpeed = 0;
  if ((proxSensors.countsFrontWithLeftLeds()  >= 4) || (proxSensors.countsFrontWithRightLeds() >= 4))
  {
    if (x < 0)
    {
        rightSpeed = 0;
        leftSpeed = 150;
      //buzzer.playFrequency(440, 200, 15);
    }
    else if  (x > 0)
    {
      //buzzer.playFrequency(440, 200, 15);
        rightSpeed = 150;
        leftSpeed = 0;
    }
    else
    {
      //buzzer.playFrequency(440, 200, 15);
       rightSpeed = 100;
       leftSpeed = 100;
    }

  }/*NOT AVOID MODE : Sets speed to 90 on both wheels making it move forward while there is no obstacle on the way.*/
  else
  {
      rightSpeed = 100;
      leftSpeed = 100;
  }
  int wait = 10;
  if (lineSensorValues[0] >= 290 )
  {
     motors.setRightSpeed(-100);
     motors.setLeftSpeed(-100);
     delay(1000);
     motors.setRightSpeed(85);
     motors.setLeftSpeed(-85);
     //delay(1000);
     
     delay(1000);
   
  }
  else if (lineSensorValues[4] >= 290)
  {
    motors.setRightSpeed(-100);
     motors.setLeftSpeed(-100);
     delay(1000);
     motors.setRightSpeed(85);
     motors.setLeftSpeed(-85);
 
     delay(1000);
  
  
   
  }

  else if ((lineSensorValues[4] >= 290) && (lineSensorValues[0] >= 290))
  {
     motors.setRightSpeed(-100);
     motors.setLeftSpeed(-100);
     delay(5000);
     motors.setRightSpeed(85);
     motors.setLeftSpeed(-85);
   
     delay(1000);
   
  
  }
  else
  {
    motors.setRightSpeed(rightSpeed);
    motors.setLeftSpeed(leftSpeed);
    delay(wait);
  }
 }

  else 
  {
    
     motors.setRightSpeed(0);
     motors.setLeftSpeed(0);
     lcd.clear();
     lcd.print("Finished");
   
  }
}


