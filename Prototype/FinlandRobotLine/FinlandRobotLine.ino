
/* 
 Stepper Motor Control - one revolution
 
 This program drives a unipolar or bipolar stepper motor. 
 The motor is attached to digital pins 8 - 11 of the Arduino.
 
 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.  
 
  
 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe
 
 */
#include <Servo.h>
#include <AccelStepper.h>

const int motorAPin1 = 2;
const int motorAPin2 = 3;
const int motorAPin3 = 4;
const int motorAPin4 = 5;
const int motorBPin1 = 6;
const int motorBPin2 = 7;
const int motorBPin3 = 8;
const int motorBPin4 = 9;
// initialize the stepper library:
AccelStepper motorA(AccelStepper::FULL4WIRE, motorAPin1, motorAPin3, motorAPin2, motorAPin4);
AccelStepper motorB(AccelStepper::FULL4WIRE, motorBPin1, motorBPin3, motorBPin2, motorBPin4);

// Calibration
const int stepsPrRevolution = 2048;
const int mmPrRevolution = 258;
const int stepsPrFullTurn = 3720;

const int servoPin = 10;
const int servoForward = 95;
const int servoLeft = 55;
const int servoRight = 135;
Servo servo;


//Light sensors
const int leftLightPin = A1;
const int rightLightPin = A2;
const int leftMin = 70;
const int leftMax = 172;
const int rightMin = 89;
const int rightMax = 218;
const int leftRange = (leftMax - leftMin) / 3;
const int rightRange = (rightMax - rightMin) / 3;
const int leftDark = leftMin + leftRange;
const int leftLight = leftMax - leftRange;
const int rightDark = rightMin + rightRange;
const int rightLight = rightMax - rightRange;

int motorAState = 0; // 0 = STOPPED, 1 = FORWARDS, -1 = BACKWARDS
int motorBState = 0; // 0 = STOPPED, 1 = FORWARDS, -1 = BACKWARDS

void setup() {
  motorA.setMaxSpeed(500.0);
  motorA.setAcceleration(300.0);
  
  motorB.setMaxSpeed(500.0);
  motorB.setAcceleration(300.0);
 
  // setup servo
  servo.attach(servoPin);
  servo.write(servoForward);
  
  // initialize the serial port:
  Serial.begin(9600);
  delay(5000);
}

void loop()
{
  //light values
  int value = readLightSum();
  //Serial.println(value);

  if(value == -1)
  {
    motorBForward();
    motorAStop();
  }
  else
  {
    motorAForward();
    motorBStop();
  }
  
  for(int i = 0; i < 20; ++i)
  {
    updateMotors();
  }
  /*
  servo.write(servoForward);
  delay(1000);
  servo.write(servoLeft);
  delay(1000);
  servo.write(servoRight);
  delay(1000);
  */
}



// Motor control

void motorAStop()
{
  motorAState = 0;
  motorA.stop();
}

void motorBStop()
{
  motorBState = 0;
  motorB.stop();
}

void motorAForward()
{
  motorAState = 1;
  motorA.moveTo(motorA.currentPosition());
  motorA.move(-stepsPrRevolution);
}

void motorBForward()
{
  motorBState = 1;
  motorB.moveTo(motorB.currentPosition());
  motorB.move(stepsPrRevolution);
}

void motorABackward()
{
  motorAState = -1;
  motorA.moveTo(motorA.currentPosition());
  motorA.move(stepsPrRevolution);
}

void motorBBackward()
{
  motorBState = -1;
  motorB.moveTo(motorB.currentPosition());
  motorB.move(-stepsPrRevolution);
}

void updateMotors()
{
  if(motorAState == 1 && motorA.distanceToGo() < stepsPrRevolution)
  {
    motorA.move(-stepsPrRevolution);
  }
  if(motorAState == -1 && motorA.distanceToGo() < stepsPrRevolution)
  {
    motorA.move(stepsPrRevolution);
  }
  if(motorBState == 1 && motorB.distanceToGo() < stepsPrRevolution)
  {
    motorB.move(stepsPrRevolution);
  }
  if(motorBState == -1 && motorB.distanceToGo() < stepsPrRevolution)
  {
    motorB.move(-stepsPrRevolution);
  }
  motorA.run();
  motorB.run();
}

int readLeftLight()
{
  int value = analogRead(leftLightPin);
  if(value <= leftDark) return -1;
  if(value >= leftLight) return 1;
  return 0;
}

int readRightLight()
{
  int value = analogRead(rightLightPin);
  if(value <= rightDark) return -1;
  if(value >= rightLight) return 1;
  return 0;
}

int readLightSum()
{
  int left = analogRead(leftLightPin);
  int right = analogRead(rightLightPin);
  int value = left + right;
  //Serial.println(left);
  //Serial.println(right);
  //Serial.println(value);
  //delay(100);
  if(value <= 340) return -1;
  return 1;
}

