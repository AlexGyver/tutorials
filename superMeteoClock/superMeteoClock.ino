#define THERM_PIN A0
#define SERVO_PIN 12
#define R_PIN 11
#define LED_PIN 9
#define G_PIN 6
#define B_PIN 5
#define RELAY_PIN 4
#define BTN_PIN 3
#define TACHO_PIN 2

#include <GyverOS.h>
GyverOS<9> OS;

#include <GyverNTC.h>
GyverNTC therm(THERM_PIN, 10000, 3950);
float thermAverage = 0;

#include <Tachometer.h>
Tachometer tacho;

#include <microDS3231.h>
MicroDS3231 rtc;

#include <Servo.h>
Servo servo;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // адрес дисплея 0x3f или 0x27

#include <EncButton.h>
EncButton<EB_TICK, BTN_PIN> btn(INPUT_PULLUP); 

#include <GRGB.h>
GRGB led(COMMON_CATHODE, R_PIN, G_PIN, B_PIN);

float thr = 30;

void setup() {
  Serial.begin(9600);
  attachInterrupt(0, isr, FALLING);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  if (rtc.lostPower()) {  //  при потере питания
    rtc.setTime(COMPILE_TIME);  // установить время компиляции
  }
  servo.attach(SERVO_PIN);

  lcd.init();           // инициализация
  lcd.backlight();      // включить подсветку

  OS.attach(0, task0, 80);
  OS.attach(1, task1, 500);
  OS.attach(2, task2, 1000);
  OS.attach(3, task3, 500);
  OS.attach(4, task4, 1000);
  OS.attach(5, task5, 1000);
  OS.attach(6, task6, 500);
  
}

void isr() {
  tacho.tick();
}

void task0() {
  static float average = 0;
  static byte counter = 0;
  if (++counter >= 10) {
    counter = 0;
    thermAverage = average / 10.0;
    average = 0;
  }
  average += therm.getTempAverage();
}

void task1() {
  Serial.print(thermAverage);
  Serial.print(',');
  Serial.println(tacho.getHz());
}

void task2() {
  if (thermAverage >= thr) digitalWrite(RELAY_PIN, 1);
  else digitalWrite(RELAY_PIN, 0);
}

void task3() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void task4() {
  servo.write(map(rtc.getSeconds(), 0, 59, 0, 180));
}

void task5() {
  lcd.home();
  lcd.print(rtc.getTimeString());
  lcd.setCursor(0, 1);
  lcd.print(thermAverage);
  lcd.print('(');
  lcd.print(thr);
  lcd.print(')');
  lcd.print(' ');
  lcd.print(digitalRead(RELAY_PIN) ? "ON " : "OFF");
}

void task6() {
  led.setWheel8(map(round(thermAverage), 25, 35, 80, 0));
}

void loop() {
  OS.tick();
  
  btn.tick();
  if (btn.isClick()) {
    static bool flag = 0;
    if (flag) OS.stop(3);
    else OS.start(3);
    flag = !flag;
  }

  static float dir = 0.5;
  if (btn.isPress()) {
    dir = -dir;
  }

  if (btn.isStep()) {
    thr += dir;
    OS.exec(5);
  }
}
