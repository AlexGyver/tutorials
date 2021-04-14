/*
    Делаем простую гирлянду на программном ШИМ
*/

#include <GyverTimers.h>  // Либа прерываний по таймеру
#include "FastIO.h"       // Либа быстрого ввода/вывода
#include "GyverButton.h"  // Либа для кнопок

#define LEDS 10     // Кол-во ледов
#define BTN_PIN 2   // Пин кнопки (внутренняя подтяжка)
const uint8_t pins[] = {A0, A1, A2, A3, A4, 5, 6, 7, 8, 9}; // Список пинов

GButton btn(BTN_PIN);
volatile int colors[LEDS]; // Массив яркостей ледов
uint8_t mode = 0;          // Переменная хранящее текущий режим

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP); // Кнопка с подтяжкой

  for (uint8_t i = 0; i < LEDS; i++) {
    pinMode(pins[i], OUTPUT);     // Все пины ледов как выходы
    colors[i] = 0;                // Яркость всех ледов = 0
  }

  Timer2.setFrequency(40000);     // Заводим прерывания таймера 2 на 40кгц
  Timer2.enableISR();             // Вкл. прерывания таймера
}

void loop() {
  static uint32_t timer = millis(); // Миллис таймер на 30мс
  if (millis() - timer >= 30) {
    timer = millis();
    switch (mode) {                 // Выбор текущего режима
      case 0: confetti(); break;    // Выводим конфетти
      case 1: track(); break;       // Выводим бегущий огонёк
      case 2: fade(); break;        // Выводим обычный Fade
    }
  }

  btn.tick();                   // Проверка состояния кнопки
  if (btn.isClick()) {          // Если кнопка нажата
    if (++mode >= 3) mode = 0;  // След. Режим + переполнение режимов
    for (uint8_t i = 0; i < LEDS; i++) colors[i] = 0; // Сбросить яркость ледов в 0
  }
}

/*
    Эффект fade
*/
int fadeAmount = 5;
int bright = 0;
void fade() {
  bright = bright + fadeAmount;
  if (bright <= 0 || bright >= 255) {
    fadeAmount = -fadeAmount;
  }
  for (int i = 0; i < LEDS; i++) {
    colors[i] = bright;
  }
}

/*
    Эффект confetti
*/
void confetti() {
  for (int i = 0; i < LEDS; i++) {
    if (colors[i] == 0 && ((int)random(0, 15) == 0) ) {
      colors[i] = 255;
    }
    colors[i] = max(colors[i] -= 10, 0);
  }
}

/*
    Эффект track
*/
int trackPos = 0;
int trackVel = 4;
void track() {
  trackPos += trackVel;
  if (trackPos < 0 || trackPos >= LEDS * 10) trackVel = -trackVel;
  colors[constrain(trackPos / 10, 0, LEDS - 1)] = 255;
  for (int i = 0; i < LEDS; i++) {
    colors[i] = max(colors[i] -= 15, 0);
  }
}


/*
   Софтверный ШИМ
*/
void pwmTick() {
  static volatile uint8_t counter = 0;  // Счетчик
  if (counter == 0) {                   // Переполнение счетчика
    for (uint8_t i = 0; i < LEDS; i++) { // Прогоняем все леды
      if (colors[i] > 0) fastWrite(pins[i], HIGH); // Если яркость != 0, вкл лед (быстрый digitalWrite)
    }
  }
  for (uint8_t i = 0; i < LEDS; i++) {   // Прогоняем все леды
    if (counter == colors[i]) {          // Если досчитали до яркости текущего леда
      fastWrite(pins[i], LOW);           // Выкл лед
    }
  }
  counter++;                             // Инкремент счетчика
}

ISR(TIMER2_A) {
  pwmTick();    // Тикер ШИМ в прерывании таймера (можно перенести код тикера сюда)
}
