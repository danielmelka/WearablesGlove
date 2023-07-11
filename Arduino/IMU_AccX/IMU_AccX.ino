/*
   Arduino and MPU6050 Accelerometer and Gyroscope Sensor Tutorial
   by Dejan, https://howtomechatronics.com
*/
#include <Wire.h>
const int MPU = 0x68; // MPU6050 I2C address
float prevAccXAvg, AccX, AccXCorr, AccY, AccZ, AccXAvg;
float prevXVel = 0, xVel = 0, yVel = 0, zVel = 0;
float xPos = 0;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, AccErrorZ, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
const unsigned int FILTER_SIZE = 10, VEL_COUNT_MAX = 50, ACC_COUNT_MAX = 50;
unsigned int count = 0, velCount = 0, accCount = 0;
float xFilter[FILTER_SIZE], yFilter[FILTER_SIZE], zFilter[FILTER_SIZE]; 
int c = 0;
void setup() {
  Serial.begin(19200); // 9600???
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  /*
  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(0x10);                   // Set the register bits as 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
  delay(20);
  */
  // Call this function if you need to get the IMU error values for your module
  calculate_IMU_error();
  delay(20);
}
void loop() {
  // === Read acceleromter data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 - AccErrorX; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 - AccErrorY; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 - AccErrorZ; // Z-axis value
  // Calculating Roll and Pitch from the accelerometer data
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI);// - AccErrorX; // AccErrorX ~(-0.20) See the calculate_IMU_error()custom function for more details
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI);// - AccErrorY; // AccErrorY ~(-4.74)
  // === Read gyroscope data === //
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  // Correct the outputs with the calculated error values
  GyroX = GyroX - GyroErrorX; // GyroErrorX ~(2.03)
  GyroY = GyroY - GyroErrorY; // GyroErrorY ~(-5.80)
  GyroZ = GyroZ - GyroErrorZ; // GyroErrorZ ~ (2.66)
  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  yaw =  yaw + GyroZ * elapsedTime;
  // Complementary filter - combine acceleromter and gyro angle values
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

  // Calculate Velocity

  AccXAvg = calcRollingFilter("x");
  
  detectResetVel();
//  detectResetAcc();

  if (abs(AccXAvg) > 0.01) {
    xVel += AccXAvg * elapsedTime * 9.81;
  }

  xPos += xVel * elapsedTime;
  
  // Print the values on the serial monitor
//    Serial.print(roll);
//    Serial.print("\t/ ");
//    Serial.print(pitch);
//    Serial.print("\t/ ");
//    Serial.print(yaw);
//    Serial.print("\t/ ");
     Serial.print(AccX);
     Serial.print("\t/ ");
//    Serial.print(AccY);
//    Serial.print("\t/ ");
//    Serial.println(AccZ);

     Serial.print(AccXAvg);
     Serial.print("\t/ ");
//    Serial.print(abs(AccXAvg));
//    Serial.print("\t/ ");
     Serial.print(xVel);
     Serial.print("\t/ ");
     Serial.println(xPos);
}

void calculate_IMU_error() {
  // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
  // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
  // Read accelerometer values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    // Sum all readings
    AccErrorX = AccErrorX + AccX;
    AccErrorY = AccErrorY + AccY;
    AccErrorZ = AccErrorZ + AccZ;
//    AccAngleErrorX = AccAngleErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
//    AccAngleErrorY = AccAngleErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    c++;
  }
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  AccErrorZ = AccErrorZ / 200;
//  AccAngleErrorX = AccAngleErrorX / 200;
//  AccAngleErrorY = AccAngleErrorY / 200;
  c = 0;
  // Read gyro values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();
    // Sum all readings
    GyroErrorX = GyroErrorX + (GyroX / 131.0);
    GyroErrorY = GyroErrorY + (GyroY / 131.0);
    GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
    c++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;
  // Print the error values on the Serial Monitor
  // Serial.print("AccErrorX: ");
  // Serial.println(AccErrorX);
  // Serial.print("AccErrorY: ");
  // Serial.println(AccErrorY);
  // Serial.print("GyroErrorX: ");
  // Serial.println(GyroErrorX);
  // Serial.print("GyroErrorY: ");
  // Serial.println(GyroErrorY);
  // Serial.print("GyroErrorZ: ");
  // Serial.println(GyroErrorZ);
}

float calcRollingFilter(String dir) {
  float sum = 0;
  if (dir.compareTo("x") == 0) {
    xFilter[count % FILTER_SIZE] = AccX + AccXCorr;
    for (float f: xFilter) {
      sum += f;
    }
    count++;
    return sum/FILTER_SIZE;
  } else if (dir.compareTo("y") == 0) {
    
  } else if (dir.compareTo("z") == 0) {
    
  } else {
    while(true) {
      Serial.println("incorrect direction value");
    }
  }
}

void detectResetAcc() {
  if (prevAccXAvg < AccXAvg + 0.005 && prevAccXAvg > AccXAvg - 0.005){
    accCount++;
  } else {
    accCount = 0;
  }
  if (accCount == ACC_COUNT_MAX){
    AccXCorr = -AccXAvg;
    accCount = 0;
  }
  prevAccXAvg = AccXAvg;
}

void detectResetVel() {
  if (prevXVel == xVel){
    velCount++;
  } else {
    velCount = 0;
  }
  if (velCount == VEL_COUNT_MAX){
    xVel = 0;
    velCount = 0;
  }
  prevXVel = xVel;
}
