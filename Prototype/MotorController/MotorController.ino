
//
/*  Remote control of two stepper motors
 *  
 *    Double motor control:
 *    0 - Stop both motors
 *    1 - Both motors forward
 *    2 - Both motors backward
 *    3 - Turn left
 *    4 - Turn right
 *    
 *    Single motor control:
 *    5 - Motor A stop
 *    6 - Motor B stop
 *    7 - Motor A Forward
 *    8 - Motor B Forward
 *    9 - Motor A Backward
 *    10 - Motor B Backward
 *    
 *    Measured distances (must be stopped first)
 *    11 - Forward (param dist in cm)
 *    12 - Backward (param dist in cm)
 *    13 - Turn left (param n, Turns (n+1)/256 of a full rotation)
 *    14 - Turn right (param n, Turns (n+1)/256 of a full rotation)
 *    
 */
#include <AccelStepper.h>
#include <ByteTransfer.h>

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
const int stepsPrFullTurn = 3000;


const int inPin = 10;
const int dataPin = 11;
const int outPin = 12;

uint8_t outData;
boolean waitingToWrite;
int movingState = 0;

bool sendByte(uint8_t data)
{
  if(waitingToWrite) return false;
  outData = data;
  waitingToWrite = true;
  return true;
}

void OnDataIn(uint8_t data)
{
  if(movingState >= 11)
  {
    switch(movingState)
    {
      case 11: 
        moveForward(data);
        movingState = 1;
        break;
      case 12:
        moveBackward(data);
        movingState = 1;
        break;
      case 13:
        turnLeft(data);
        movingState = 1;
        break;
      case 14:
        turnRight(data);
        movingState = 1;
        break;
    }
    return;
  }
  switch(data)
  {
    case 0:
      movingState = 0;
      motorAStop();
      motorBStop();
      sendByte(0);
      break;
    case 1:
      movingState = 0;
      motorAForward();
      motorBForward();
      sendByte(0);
      break;
    case 2:
      movingState = 0;
      motorABackward();
      motorBBackward();
      sendByte(0);
      break;
    case 3:
      movingState = 0;
      motorAForward();
      motorBBackward();
      sendByte(0);
      break;
    case 4:
      movingState = 0;
      motorABackward();
      motorBForward();
      sendByte(0);
      break;
      //Individual motor control
    case 5:
      movingState = 0;
      motorAStop();
      sendByte(0);
      break;
    case 6:
      movingState = 0;
      motorBStop();
      sendByte(0);
      break;
    case 7:
      movingState = 0;
      motorAForward();
      sendByte(0);
      break;
    case 8:
      movingState = 0;
      motorBForward();
      sendByte(0);
      break;
    case 9:
      movingState = 0;
      motorABackward();
      sendByte(0);
      break;
    case 10:
      movingState = 0;
      motorBBackward();
      sendByte(0);
      break;
    case 11:
    case 12:
    case 13:
    case 14:
      movingState = data;
      break;
    default:
      sendByte(1);
      break;
  }
}

bool OnDataOut(uint8_t &data)
{
  if(waitingToWrite)
  {
    data = outData;
    waitingToWrite = false;
    return true;
  }
  return false;
}

ByteTransfer bt(inPin, dataPin, outPin, OnDataIn, OnDataOut);

int motorAState = 0;
int motorBState = 0;

void setup()
{
  waitingToWrite = false;
  delay(10000);
  bt.Initialize();
  motorA.setMaxSpeed(300.0);
  motorA.setAcceleration(600.0);
  motorB.setMaxSpeed(300.0);
  motorB.setAcceleration(600.0);
}

void loop()
{
  updateMotors();
  bt.Update();
}

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
  motorA.move(-stepsPrRevolution);
}

void motorBForward()
{
  motorBState = 1;
  motorB.move(stepsPrRevolution);
}

void motorABackward()
{
  motorAState = -1;
  motorA.move(stepsPrRevolution);
}

void motorBBackward()
{
  motorBState = -1;
  motorB.move(-stepsPrRevolution);
}

void updateMotors()
{
  if(movingState == 0)
  {
    if(motorAState == 1 && motorA.distanceToGo() > -stepsPrRevolution)
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
    if(motorBState == -1 && motorB.distanceToGo() > -stepsPrRevolution)
    {
      motorB.move(-stepsPrRevolution);
    }
  }
  else if(movingState == 1 && motorA.distanceToGo() == 0 && motorB.distanceToGo() == 0)
  {
      sendByte(2);
      movingState = 0;
  }
  motorA.run();
  motorB.run();
}

long cmToSteps(int cm)
{
  long mm = cm * 10;
  long steps = (mm * stepsPrRevolution) / mmPrRevolution;
  return steps;
}

void moveForward(int cm)
{
  long steps = cmToSteps(cm);
  motorA.move(-steps);
  motorB.move(steps);
}

void moveBackward(int cm)
{
  long steps = cmToSteps(cm);
  motorA.move(steps);
  motorB.move(-steps);
}

void turnLeft(int value)
{
  long steps = (value + 1);
  steps *= stepsPrFullTurn;
  steps /= 256;
  motorA.move(steps);
  motorB.move(steps);
}

void turnRight(int value)
{
  long steps = (value + 1);
  steps *= stepsPrFullTurn;
  steps /= 256;
  motorA.move(-steps);
  motorB.move(-steps);
}

