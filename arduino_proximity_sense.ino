#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define echoPin 2
#define trigPin 3

const int red_led = 5;
const int orange_led = 9;
const int buzzer = 4;

//a5 >> SCL
//a4 >> SDA

int state = LOW;

long duration;
int distance;

unsigned long previousMillis = 0;
unsigned int interval = 1000;

bool warning = false;
bool danger = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(orange_led, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  lcd.begin();

  lcd.backlight();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Starting up");
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
}
void loop() {
  unsigned long currentMillis = millis();

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);

  if (distance > 3.5 and distance <= 4) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("WARN NOT CLEAR");
    lcd.setCursor(1, 1);
    lcd.print(distance);
    lcd.print("Cm");
    interval = 1000;
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      if (state == LOW) {
        noTone(buzzer);
      }
      state = HIGH;
    } else {
      tone(buzzer, 1000);
      state = LOW;
    }

    digitalWrite(orange_led, HIGH);
    digitalWrite(red_led, LOW);
  } else if (distance <= 3.5) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("RISK NOT CLEAR");
    lcd.setCursor(1, 1);
    lcd.print(distance);
    lcd.print("Cm");

    interval = 600;
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      if (state == LOW) {
        noTone(buzzer);
      }
      state = HIGH;
    } else {
      tone(buzzer, 1000);
      state = LOW;
    }
    digitalWrite(orange_led, LOW);
    digitalWrite(red_led, HIGH);
  } else {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("CLEAR");
    lcd.setCursor(1, 1);
    lcd.print(distance);
    lcd.print("Cm");
    noTone(buzzer);
    digitalWrite(orange_led, LOW);
    digitalWrite(red_led, LOW);
  }



}
//  if (warning == true and danger == false){
//    interval =1000;
//  }else{
//    interval = 800;
//  }
//  Serial.println(" cm");


//  if (currentMillis - previousMillis >= interval) {
//    // save the last time you blinked the LED
//    previousMillis = currentMillis;
//
//    // if the LED is off turn it on and vice-versa:
//    if (ledState == LOW) {
//      ledState = HIGH;
//    } else {
//      ledState = LOW;
//    }
//
//    // set the LED with the ledState of the variable:
//    digitalWrite(ledPin, ledState);
//  }
