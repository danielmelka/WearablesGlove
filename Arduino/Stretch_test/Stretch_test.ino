int stretchsensor1 = A0;
int stretchsensor2 = A1;
int stretchsensor3 = A2;
int stretchsensor4 = A3;
int stretchsensor5 = A4;

void setup() {
  pinMode(stretchsensor1, INPUT);
  pinMode(stretchsensor2, INPUT);
  pinMode(stretchsensor3, INPUT);
  pinMode(stretchsensor4, INPUT);
  pinMode(stretchsensor5, INPUT);
  analogReference(DEFAULT);
  Serial.begin(9600);
}

void loop() {
  int val1 = analogRead(stretchsensor1);
  int val2 = analogRead(stretchsensor2);
  int val3 = analogRead(stretchsensor3);
  int val4 = analogRead(stretchsensor4);
  int val5 = analogRead(stretchsensor5);
  
  Serial.print(val1);
  Serial.print(", ");
  Serial.print(val2);
  Serial.print(", ");
  Serial.print(val3);
  Serial.print(", ");
  Serial.print(val4);
  Serial.print(", ");
  Serial.println(val5);
}
