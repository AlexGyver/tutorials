// пример диммера на симисторе с внешним детектором нуля
// или готовый китайский модуль https://ali.ski/xwpCCe
// используется библиотека GyverTimers (минимум версия 1.5)
// по аналогии можно сделать диммер на любом таймере ардуино
// в том числе любом из 6-ти таймеров для Mega
// многоканальный диммер. Тут пример с разрешением 256 (значения 0-255)
#define ZERO_PIN 2    // пин детектора нуля
#define INT_NUM 0     // соответствующий ему номер прерывания
#define DIM_AMOUNT 3  // количество диммеров

const byte dimPins[] = {3, 4, 5}; // их пины
#include <GyverTimers.h>    // библиотека таймера
int dimmer[DIM_AMOUNT];     // переменная диммера
volatile int counter = 0;   // счётчик цикла

void setup() {
  pinMode(ZERO_PIN, INPUT_PULLUP);
  for (byte i = 0; i < DIM_AMOUNT; i++) pinMode(dimPins[i], OUTPUT);
  attachInterrupt(INT_NUM, isr, FALLING); // для самодельной схемы ставь RISING
  Timer2.enableISR();
  // 37 мкс - период прерываний для 255 шагов и 50 Гц
  // для 60 Гц ставь число 31
  Timer2.setPeriod(37); 
  Serial.begin(9600);
}

void loop() {
  // задаём значение
  //dimmer[0] = map(analogRead(A0), 0, 1023, 0, 9500);
  dimmer[0] = 50;
  dimmer[1] = 120;
  dimmer[2] = 190;
  delay(100); // в реальном коде задержек быть не должно
}

// прерывание детектора нуля
void isr() {
  counter = 255;
  Timer2.restart();
}

// прерывание таймера
ISR(TIMER2_A) {
  for (byte i = 0; i < DIM_AMOUNT; i++) {
    if (counter == dimmer[i]) digitalWrite(dimPins[i], 1);  // на текущем тике включаем
    else if (counter == dimmer[i] - 1) digitalWrite(dimPins[i], 0);  // на следующем выключаем
  }
  counter--;
}
