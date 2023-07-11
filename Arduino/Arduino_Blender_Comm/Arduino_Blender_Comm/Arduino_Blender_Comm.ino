void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.write(33);

// int test = 90;
// byte buf[2];
// buf[1] = (byte) test;
// buf[0] = (byte) (test >> 8);
// Serial.write(buf, 2);

 float pitch = 90.01;
 byte * pbuf = (byte *) &pitch;
 Serial.write(pbuf, 4);

  float roll = 0.01;
 byte * rbuf = (byte *) &roll;
 Serial.write(rbuf, 4);

  float yaw = 123.45;
 byte * ybuf = (byte *) &yaw;
 Serial.write(ybuf, 4);
 
//  Serial.write(49);
//  Serial.write(48);
//  Serial.print(10);
//  Serial.print('!');

}
