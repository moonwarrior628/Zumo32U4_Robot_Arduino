// The Group members for this program is T. R. and M. S.
//This program is designed to determine whether the robot is on an incline of ~30 degrees. Once the robot determines whether it is on an incline or not
//The robot will then drive up the hill until it reaches the flat surface. When it reaches the flat surface at the top of the hill, the robot will stop
//all motors and display the words "STOPPED" on the LCD. 
//We modeled our code after the code from the Arduino program call FaceUphill, as a base for the program.

#include <Wire.h>
#include <Zumo32U4.h>

//Set the max speed for the turn radius as 200
const int16_t maxSpeed = 200;

LSM303 lsm303;
Zumo32U4Motors motors;
Zumo32U4LCD lcd;
Zumo32U4Encoders encoders;
Zumo32U4ButtonA buttonA;
void setup()
{
  // Start I2C and initialize the LSM303 accelerometer.
  Wire.begin();
  lsm303.init();
  lsm303.enableDefault();
  lcd.print("PressA");
  buttonA.waitForButton();
  lcd.clear();
  delay(5);

}

void loop()
{
  // Read the acceleration from the LSM303.
  // A value of 16384 corresponds to approximately 1 g.
  lsm303.read();
  int16_t x = lsm303.a.x;
  int16_t y = lsm303.a.y;
  int32_t magnitudeSquared = (int32_t)x * x + (int32_t)y * y;

  // Display the magnitudeSquared and turnSpeed values on the LCD
  // every 100 ms.
  //This is shown further down the code.
  static uint8_t lastDisplayTime;

  // Use the encoders to see how much we should drive forward.
  // If the robot rolls downhill, the encoder counts will become
  // negative, resulting in a positive forwardSpeed to counteract
  // the rolling.

  int16_t turnSpeed;
  int16_t forwardSpeed = -(encoders.getCountsLeft() + encoders.getCountsRight());
  forwardSpeed = constrain(forwardSpeed, -maxSpeed, maxSpeed);

  // See if we are actually on an incline using the following equation.
  // 16384 * sin(30 deg) = 8192
  // We are on an incline of more than 5 degrees, so
  // try to face uphill using a feedback algorithm.

  turnSpeed = y / 16;

  // Determine  if it is on a hill and then drive forward with out
  // correcting the robot is facing uphill
  if (magnitudeSquared > (int32_t)8192 * 8192)
  {
    forwardSpeed = 250;
    ledYellow(1);
  }
  //Determines if the robot is both turning and on a hill

  else if (turnSpeed >= -50 && turnSpeed <= 50 && magnitudeSquared > (int32_t)8192 * 8192)
  {
    forwardSpeed = 200;
  }
  else
  {
    // When its not on a noticeable incline, so don't turn and don't drive forward.
    // Display that the robot has stopped.

    turnSpeed = 0;
    forwardSpeed = 0;
    ledYellow(0);
    lcd.clear();
    lcd.print("STOPPED");
  }

  // To face uphill, we need to turn so that the X acceleration
  // is negative and the Y acceleration is 0.  Therefore, when
  // the Y acceleration is positive, we want to turn to the
  // left (counter-clockwise).
  int16_t leftSpeed = forwardSpeed - turnSpeed;
  int16_t rightSpeed = forwardSpeed + turnSpeed;


  // Constrain the speeds to be between -maxSpeed and maxSpeed.
  leftSpeed = constrain(leftSpeed, -maxSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, -maxSpeed, maxSpeed);

  // Display the magnitudeSquared and turnSpeed values on the LCD
  // every 100 ms.
  if ((uint8_t)(millis() - lastDisplayTime) > 100)
  {
    lastDisplayTime = millis();
    lcd.gotoXY(0, 0);
    lcd.print(magnitudeSquared / 100000);
    lcd.print(F("       "));
    lcd.gotoXY(0, 1);
    lcd.print(turnSpeed);
    lcd.print(F("       "));
  }

  motors.setSpeeds(leftSpeed, rightSpeed);
}


