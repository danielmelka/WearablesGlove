#include <LiquidCrystal.h>

// Initialize Variables
int Contrast = 75;
int flexsensor = A0;
int motor = 13;
LiquidCrystal lcd (12, 11, 5, 4, 3, 7);

void setup() {
  // put your setup code here, to run once:
  pinMode(flexsensor, INPUT);
  analogReference(DEFAULT);
  pinMode(motor, OUTPUT);
  Serial.begin(9600);

  // LCD
  analogWrite(6, Contrast);
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = analogRead(flexsensor);
  Serial.println(value);
  if (value < 400 && value > 200) {
    Serial.println("run motor");
    digitalWrite(motor, HIGH);
    delay(1000);

    // LCD Printing - First Line
    lcd.setCursor(0,0);
    lcd.print("WashU Robotics: ");

    // LCD Printing - Second Line
    lcd.setCursor(0, 1);
    lcd.print("Stand Up     ");
  }
  else if (value < 200) {
    Serial.println("run motor");
    digitalWrite(motor, HIGH);
    delay(1000);

    // LCD Printing - First Line
    lcd.setCursor(0,0);
    lcd.print("WashU Robotics: ");

    // LCD Printing - Second Line
    lcd.setCursor(0, 1);
    lcd.print("Get Checked   ");

  }
  else {
    Serial.println(value);
    digitalWrite(motor, LOW);
    delay(1000);

        // LCD Printing - First Line
    lcd.setCursor(0,0);
    lcd.print("WashU Robotics: ");

    // LCD Printing - Second Line
    lcd.setCursor(0, 1);
    lcd.print("Standing    ");
  }
}
