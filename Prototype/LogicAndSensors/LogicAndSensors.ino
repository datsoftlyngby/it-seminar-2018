/*  Remote control of motors
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
 *    Measured distances (must be stopped first):
 *    11 - Forward (param dist in cm)
 *    12 - Backward (param dist in cm)
 *    13 - Turn left (param n, Turns (n+1)/256 of a full rotation)
 *    14 - Turn right (param n, Turns (n+1)/256 of a full rotation)    
 */


#include <ByteTransfer.h>


//Light sensor
#define LEFT_LIGHT_PIN A1
#define RIGHT_LIGHT_PIN A2
#define LEFT_WHITE 173
#define RIGHT_WHITE 199
#define LEFT_BLACK 47
#define RIGHT_BLACK 81

#define LEFT_LIGHT_RANGE (LEFT_WHITE - LEFT_BLACK)
#define RIGHT_LIGHT_RANGE (RIGHT_WHITE - RIGHT_BLACK)

int leftLight()
{
  int raw = analogRead(LEFT_LIGHT_PIN);
  return map(raw, LEFT_BLACK, LEFT_WHITE, 0, 100);
}

int rightLight()
{
  int raw = analogRead(RIGHT_LIGHT_PIN);
  return map(raw, RIGHT_BLACK, RIGHT_WHITE, 0, 100);
}

#define LINE_STEER 10

//Set up pins for ByteTransfer
const int inPin = 4;
const int dataPin = 5;
const int outPin = 6;

uint8_t outData;
boolean waitingToWrite;
boolean paramWaiting;
uint8_t parameter;

int motorAState = 0;
int motorBState = 0;

void sendByte(uint8_t data)
{
  outData = data;
  waitingToWrite = true;
}

void OnDataIn(uint8_t data)
{
  if(data == 2)
  {
    onFinishMove();
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
  else if(paramWaiting)
  {
    data = parameter;
    paramWaiting = false;
    return true;
  }
  return false;
}

ByteTransfer bt(inPin, dataPin, outPin, OnDataIn, OnDataOut);

String inString = "";

void setup()
{
  Serial.begin(9600);
  delay(10000);
  bt.Initialize();
  waitingToWrite = false;
  paramWaiting = false;
}

int curState = 0;

void loop()
{
  if(!waitingToWrite && !paramWaiting)
  {
    updateLoop(); 
  }
  bt.Update();
}

void updateLoop()
{
  followLine();
}


void followLine()
{
  int left = leftLight();
  int right = rightLight();
  Serial.print("Left: ");
  Serial.print(left);
  Serial.print(" Right: ");
  Serial.println(right);

  int dist = abs(left-right);
  if(dist > LINE_STEER)
  {
    //We have to correct
    if(left > right)
    {
      motorAForward();
      motorBStop();
    }
    else
    {
      motorAStop();
      motorBForward();
    }
  }
  else
  {
    /*
    if(left + right < 100)
    {
      //On black
      moveForward();
    }
    else
    {
      //On white
      moveBackward();
    }
    */
    moveForward();
  }
}

void halt()
{
  if(curState != 0)
  {
    sendByte(0);
    curState = 0;
    motorAState = 0;
    motorBState = 0;
  }
}

void moveForward()
{
  if(curState != 1)
  {
    sendByte(1);
    curState = 1;
    motorAState = 1;
    motorBState = 1;
  }
}

void moveBackward()
{
  if(curState != 2)
  {
    sendByte(2);
    curState = 2;
    motorAState = -1;
    motorBState = -1;
  }
}

void turnLeft()
{
  if(curState != 3)
  {
    sendByte(3);
    curState = 3;
    motorAState = -1;
    motorBState = 1;
  }
}

void turnRight()
{
  if(curState != 4)
  {
    sendByte(4);
    curState = 4;
    motorAState = 1;
    motorBState = -1;
  }
}

void motorAStop()
{
  curState = 5;
  if(motorAState != 0)
  {
    motorAState = 0;
    sendByte(5);
  }
}

void motorBStop()
{
  curState = 6;
  if(motorBState != 0)
  {
    motorBState = 0;
    sendByte(6);
  }
}

void motorAForward()
{
  curState = 7;
  if(motorAState != 1)
  {
    motorAState = 1;
    sendByte(7);
  }
}

void motorBForward()
{
  curState = 8;
  if(motorBState != 1)
  {
    motorBState = 1;
    sendByte(7);
  }
}

void motorABackward()
{
  curState = 9;
  if(motorAState != -1)
  {
    motorAState = -1;
    sendByte(9);
  }
}

void motorBBackward()
{
  curState = 10;
  if(motorBState != -1)
  {
    motorBState = -1;
    sendByte(10);
  }
}



void moveForward(uint8_t cm)
{
  curState = 11;
  sendByte(11);
  parameter = cm;
  paramWaiting = true;
}

void moveBackward(uint8_t cm)
{
  curState = 12;
  sendByte(12);
  parameter = cm;
  paramWaiting = true;
}

void turnLeft(int value)
{
  curState = 13;
  sendByte(13);
  parameter = value - 1;
  paramWaiting = true;
}

void turnRight(int value)
{
  curState = 14;
  sendByte(14);
  parameter = value - 1;
  paramWaiting = true;
}

void onFinishMove()
{
  curState = 0;
}





