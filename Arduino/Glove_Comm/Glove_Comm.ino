#include <Wire.h>
#include <JY901.h>
/*
Test on Uno R3.
JY901   UnoR3
TX <---> 0(Rx)
*/

int rxon = 7;
int txon = 9;
int fin0 = A0;
int fin1 = A1;
int fin2 = A2;
int fin3 = A3;
int fin4 = A4;

int f0high = 700;
int f0low = 600;
int f1high = 700;
int f1low = 600;
int f2high = 700;
int f2low = 600;
int f3high = 700;
int f3low = 600;
int f4high = 700;
int f4low = 600;

void setup() 
{
  pinMode(rxon, OUTPUT);
  pinMode(txon, OUTPUT);
  pinMode(fin0, INPUT);
  pinMode(fin1, INPUT);
  pinMode(fin2, INPUT);
  pinMode(fin3, INPUT);
  pinMode(fin4, INPUT);
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

  int f0read = analogRead(fin0);
  float f0 = (float)(f0high - f0read)/(f0high - f0low);
  if (f0 > 1){
    f0 = 1;
  }
  if (f0 < 0){
    f0 = 0;
  }
  
  int f1read = analogRead(fin1);
  float f1 = (float)(f1high - f1read)/(f1high - f1low);
  if (f1 > 1){
    f1 = 1;
  }
  if (f1 < 0){
    f1 = 0;
  }

  int f2read = analogRead(fin2);
  float f2 = (float)(f2high - f2read)/(f2high - f2low);
  if (f2 > 1){
    f2 = 1;
  }
  if (f2 < 0){
    f2 = 0;
  }

  int f3read = analogRead(fin3);
  float f3 = (float)(f3high - f3read)/(f3high - f3low);
  if (f3 > 1){
    f3 = 1;
  }
  if (f3 < 0){
    f3 = 0;
  }

  int f4read = analogRead(fin4);
  float f4 = (float)(f4high - f4read)/(f4high - f4low);
  if (f0 > 1){
    f4 = 1;
  }
  if (f4 < 0){
    f4 = 0;
  }
  
  


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
