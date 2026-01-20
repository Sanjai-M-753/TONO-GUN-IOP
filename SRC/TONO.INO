/*
-------------------------------------------------
 Project : TONO-GUN
 Description : Miniature Non-Contact Air-Puff Tonometer
 Author : Sanjai Prasath
 Purpose : Research & Prototype (Non-clinical)
-------------------------------------------------
*/

#include <Wire.h>
#include <Adafruit_SSD1306.h>

// -------- OLED CONFIG --------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// -------- PIN DEFINITIONS --------
#define AIR_PUMP_PIN 9
#define IR_SENSOR_PIN A0
#define PRESSURE_SENSOR_PIN A1
#define BUTTON_PIN 2
#define BUZZER_PIN 8

// -------- VARIABLES --------
int irValue = 0;
int pressureValue = 0;
float iopValue = 0;

bool measurementDone = false;

// -------- SETUP --------
void setup() {
  pinMode(AIR_PUMP_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(AIR_PUMP_PIN, LOW);

  Serial.begin(9600);

  // OLED INIT
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 10);
  display.println("TONO-GUN");
  display.println("Non-Contact");
  display.println("IOP Measurement");
  display.display();

  delay(2000);
}

// -------- LOOP --------
void loop() {

  if (digitalRead(BUTTON_PIN) == LOW && !measurementDone) {
    measurementDone = true;
    measureIOP();
  }

  if (digitalRead(BUTTON_PIN) == HIGH) {
    measurementDone = false;
  }
}

// -------- IOP MEASUREMENT FUNCTION --------
void measureIOP() {

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Measuring IOP...");
  display.display();

  // Air Puff Trigger
  digitalWrite(AIR_PUMP_PIN, HIGH);
  delay(80);                     // Air puff duration
  digitalWrite(AIR_PUMP_PIN, LOW);

  delay(100);

  // Sensor Readings
  irValue = analogRead(IR_SENSOR_PIN);
  pressureValue = analogRead(PRESSURE_SENSOR_PIN);

  // Simple estimation formula (prototype logic)
  iopValue = map(irValue, 200, 800, 10, 30);

  if (iopValue < 5) iopValue = 5;
  if (iopValue > 40) iopValue = 40;

  // Display Results
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("IOP Result:");
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(iopValue);
  display.print(" mmHg");
  display.setTextSize(1);
  display.display();

  Serial.print("IR Value: ");
  Serial.println(irValue);
  Serial.print("Estimated IOP: ");
  Serial.print(iopValue);
  Serial.println(" mmHg");

  tone(BUZZER_PIN, 1000, 200);
  delay(3000);
}
