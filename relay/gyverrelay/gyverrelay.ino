#define THERM_PIN 0
#define RELAY_PIN 2
#define SETPOINT 50.0
#define HYSTER 2

#include "thermistorMinim.h"
// GND --- термистор --- A0 --- 10к --- 5V
thermistor therm(THERM_PIN, 10000, 3950);  // пин, сопротивление, бета-коэффициент

#include "GyverRelay.h"
// установка, гистерезис, направление регулирования
GyverRelay regulator;
// либо GyverRelay regulator(); без указания направления (будет REVERSE)

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);     // пин реле
  regulator.k = 8.5;              // коэффициент обратной связи (подбирается по факту)
  regulator.setpoint = SETPOINT;  // установка (ставим на SETPOINT градусов)
  regulator.hysteresis = HYSTER;  // ширина гистерезиса
}

void loop() {
  regul();
  debug();
}

void regul() {
  static uint32_t tmr;
  if (millis() - tmr > 500) {
    tmr = millis();
    regulator.input = therm.getTempAverage(); // сообщаем регулятору текущую температуру
    digitalWrite(RELAY_PIN, regulator.getResult());   // отправляем на реле (ОС работает по своему таймеру)
  }
}

void debug() {
  static uint32_t tmr;
  if (millis() - tmr > 50) {
    tmr = millis();
    Serial.print(regulator.input);    // фактическая
    Serial.print(',');
    Serial.print(SETPOINT);  // гистерезис
    Serial.print(',');
    Serial.print(SETPOINT + HYSTER);  // гистерезис
    Serial.print(',');
    Serial.print(SETPOINT - HYSTER);  // гистерезис
    Serial.print(',');
    Serial.println(regulator.output * 2 + 30); // сост. реле
  }
}
