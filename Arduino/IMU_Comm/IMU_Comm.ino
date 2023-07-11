#include <Wire.h>
#include <JY901.h>
/*
Test on Uno R3.
JY901   UnoR3
TX <---> 0(Rx)
*/

int rxon = 6;
int txon = 7;

void setup() 
{
  pinMode(rxon, OUTPUT);
  pinMode(txon, OUTPUT);
  digitalWrite(rxon, HIGH);
  digitalWrite(txon, HIGH);
  
  Serial.begin(9600);
}

void loop() 
{
  digitalWrite(rxon, HIGH);
  digitalWrite(txon, HIGH);

  float pitch = (float)JY901.stcAngle.Angle[0]/32768*180;
  float roll = (float)JY901.stcAngle.Angle[1]/32768*180;
  float yaw = (float)JY901.stcAngle.Angle[2]/32768*180;

  float f0 = 1;
  float f1 = 0.75;
  float f2 = 0.5;
  float f3 = 0.25;
  float f4 = 0;


  Serial.write(33);
  
  byte * pbuf = (byte *) &pitch;
  Serial.write(pbuf, 4);
 
  byte * rbuf = (byte *) &roll;
  Serial.write(rbuf, 4);
 
  byte * ybuf = (byte *) &yaw;
  Serial.write(ybuf, 4);

  byte * f0buf = (byte *) &f0;
  Serial.write(f0buf, 4);
  
  byte * f1buf = (byte *) &f1;
  Serial.write(f1buf, 4);
  
  byte * f2buf = (byte *) &f2;
  Serial.write(f2buf, 4);
  
  byte * f3buf = (byte *) &f3;
  Serial.write(f3buf, 4);

  byte * f4buf = (byte *) &f4;
  Serial.write(f4buf, 4);


  delay(100);
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() 
{
  while (Serial.available()) 
  {
    JY901.CopeSerialData(Serial.read()); //Call JY901 data cope function
  }
}
