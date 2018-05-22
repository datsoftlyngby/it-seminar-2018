/*  Remote control of motors
 *  
 *    0 - Stop both motors
 *    1 - Both motors forward
 *    2 - Both motors backward
 *    3 - Turn left
 *    4 - Turn right
 */
//
#define TRIGGER_PIN  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     3  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters).

#include <NewPing.h>
#include <ByteTransfer.h>


//Set up pins for ByteTransfer
const int inPin = 4;
const int dataPin = 5;
const int outPin = 6;

uint8_t outData;
boolean waitingToWrite;

//Ultra sensor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void sendByte(uint8_t data)
{
  outData = data;
  waitingToWrite = true;
}

void OnDataIn(uint8_t data)
{
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

String inString = "";

void setup()
{
  delay(10000);
  bt.Initialize();
  waitingToWrite = false;
}

void loop()
{
  if(!waitingToWrite)
  {
    //Read distance
    int dist = sonar.ping_cm();
    if(dist < 20)
    {
      sendByte(3);
    }
    else
    {
      sendByte(1);
    }
  }
  bt.Update();
}



