// пример диммера на симисторе с внешним детектором нуля
// или готовый китайский модуль https://ali.ski/xwpCCe
// используется библиотека GyverTimers (минимум версия 1.5)
// по аналогии можно сделать диммер на любом таймере ардуино
// в том числе любом из 6-ти таймеров для Mega
// вызов setPeriod оптимизирован
#define ZERO_PIN 2    // пин детектора нуля
#define INT_NUM 0     // соответствующий ему номер прерывания
#define DIMMER_PIN 4  // управляющий пин симистора

#include <GyverTimers.h>  // библиотека таймера
int dimmer;               // переменная диммера

void setup() {
  pinMode(ZERO_PIN, INPUT_PULLUP);
  pinMode(DIMMER_PIN, OUTPUT);
  attachInterrupt(INT_NUM, isr, RISING);  // для самодельной схемы ставь FALLING
  Timer2.enableISR();
}

void loop() {
  // задаём значение 500-9300, где 500 максимум мощности, 9300 минимум!!!
  // и 500-7600 для 60 Гц в сети
  dimmer = map(analogRead(A0), 0, 1024, 500, 9300);
  delay(100); // в реальном коде задержек быть не должно
}

// прерывание детектора нуля
void isr() {
  static int lastDim;
  digitalWrite(DIMMER_PIN, 0);  // выключаем симистор
  // если значение изменилось, устанавливаем новый период
  // если нет, то просто перезапускаем со старым
  if (lastDim != dimmer) Timer2.setPeriod(lastDim = dimmer);
  else Timer2.restart();
}

// прерывание таймера
ISR(TIMER2_A) {
  digitalWrite(DIMMER_PIN, 1);  // включаем симистор
  Timer2.stop();                // останавливаем таймер
}
