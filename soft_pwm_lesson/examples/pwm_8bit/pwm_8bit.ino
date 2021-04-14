/*
    Программный ШИМ 8 бит, 3 канала
    Выводим ШИМ на пины А3,A4,A5
    Значение заполнения читаем с потенциометров на пинах A0,A1,A2
*/

#include <GyverTimers.h>  // Либа прерываний по таймеру
#include "FastIO.h"       // Либа быстрого ввода/вывода

volatile byte dutyA3 = 0; // Переменные для хранения заполнения ШИМ
volatile byte dutyA4 = 0; // Можно упаковать в массив при желании
volatile byte dutyA5 = 0;

void setup() {
  pinMode(A3, OUTPUT); // Все каналы ШИМ устанавливаются как выходы
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  Timer2.setFrequency(40000);     // Заводим прерывания таймера 2 на 40кгц
  Timer2.enableISR();             // Вкл. прерывания таймера
}

void loop() { // Получаем заполнение ШИМ с потенциометров
  dutyA3 = analogRead(A0) / 4; // У каждого канала свой потенциометр
  dutyA4 = analogRead(A1) / 4; // Преобразуем 10 бит АЦП в 8 бит ШИМ
  dutyA5 = analogRead(A2) / 4; // Делением на 4 (0...1023 -> 0...255)
  delay(100);                  // Других задач нет - можно поставить delay (иначе ставим millis() таймер)
}


/*
   Софтверный ШИМ
*/
void pwmTick() {
  static volatile uint8_t counter = 0;  // Счетчик

  if (counter == 0) {
    // Переполнение счетчика - все каналы ШИМ устанавливаются в HIGH
    if (dutyA3 > 0) fastWrite(A3, HIGH); // Устанавливаем HIGH, только если заполнение >0 (быстрый digitalWrite)
    if (dutyA4 > 0) fastWrite(A4, HIGH); // Решает проблему слабого свечения LED при заполнении = 0
    fastWrite(A5, HIGH); // Можно и не проверять на 0, если используется электродвигатель или лампа накаливания
  }

  if (counter == dutyA3) fastWrite(A3, LOW); // Проверяем все каналы на совпадение счетчика со значением заполнения
  if (counter == dutyA4) fastWrite(A4, LOW); // При совпадении переводим канал в состояние LOW
  if (counter == dutyA5) fastWrite(A5, LOW);

  counter++;                                        // Инкремент счетчика
}

ISR(TIMER2_A) {
  pwmTick();    // Тикер ШИМ в прерывании таймера (можно перенести код тикера сюда)
}
