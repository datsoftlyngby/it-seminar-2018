#include <ByteTransfer.h>

//InPin 2, DataPin 3, OutPin 4
ByteTransfer bt(2,3,4, OnDataIn, OnDataOut);
uint8_t outData;

void setup()
{
  Serial.begin(9600);
  Serial.println("Waiting 10 seconds");
  delay(10000);
  Serial.println("Initializing");
  bt.Initialize();
  Serial.println("Waiting 5 seconds");
  delay(5000);
  outData = 0;
}

void loop()
{
  //delay(1000); //Just to see what is going on...
  bt.Update();
}

void OnDataIn(uint8_t data)
{
  Serial.print("Byte in: ");
  Serial.println(data);
}

bool OnDataOut(uint8_t &data)
{
  //Send out the 7 times table
  outData += 7;
  if(outData <= 70)
  {
    data = outData;
    return true;
  }
  if(outData > 210)
  {
    outData = 0;
  }
  return false;
}
