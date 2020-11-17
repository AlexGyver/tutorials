#define THERM_PIN 0
#define RELAY_PIN 2
#define SETPOINT 50.0
#define HYSTER 2

#include "thermistorMinim.h"
// GND --- термистор --- A0 --- 10к --- 5V
thermistor therm(THERM_PIN, 10000, 3950);	// пин, сопротивление, бета-коэффициент

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
}

bool relayState = false;
void regul(float temp) {
  // таймер уже не так сильно нужен
  if (temp < (SETPOINT - HYSTER)) relayState = true;
  else if (temp > (SETPOINT + HYSTER)) relayState = false;
  
  digitalWrite(RELAY_PIN, relayState);
}

void debug(float temp) {
  static uint32_t tmr;
  if (millis() - tmr > 50) {
    tmr = millis();
    Serial.print(temp);       // фактическая
    Serial.print(',');
    Serial.print(SETPOINT);   // установка
    Serial.print(',');
    Serial.print(SETPOINT + HYSTER); // гистерезис
    Serial.print(',');
    Serial.print(SETPOINT - HYSTER); // гистерезис
    Serial.print(',');
    Serial.println(relayState * 2 + 40); // сост. реле
  }
}

void loop() {
  float temp = therm.getTempAverage();
  regul(temp);
  debug(temp);
}
