#include "Parser.h"
#include "AsyncStream.h"
AsyncStream<50> serial(&Serial, ';');

#include <Servo.h>
Servo servo;

#define R_PIN 3
#define G_PIN 5
#define B_PIN 6
#define LED_1 7
#define LED_2 8
#define SRV_PIN 9

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(5);
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  servo.attach(SRV_PIN);
}

// протокол:
// 0 - светодиод1 (0/1)
// 1 - светодиод2 (0/1)
// 2 - RGB (0..255,0..255,0..255)
// 3 - серво (0..180)

void loop() {
  // асинхронное чтение через AsyncStream
  if (serial.available()) {
    Serial.println(serial.buf);
  }
  
  /*
  // асинхронное чтение буфера сериал через костыли
  static byte prevAm = 0;
  static uint32_t tmr = 0;
  byte am = Serial.available();
  if (am != prevAm) {
    prevAm = am;
    tmr = millis();
  }
  if ((am & millis() - tmr > 10) || am > 60) {
    uint32_t us = micros();
    char str[150];
    int amount = Serial.readBytesUntil(';', str, 150);
    Serial.println(micros() - us);
    str[amount] = NULL;
    Serial.println(str);
    */
    /*
      // управление ледами и сервой
      Parser data(str, ',');
      int ints[5];
      int am = data.parseInts(ints);

      switch (ints[0]) {  // ключ
      case 0: digitalWrite(LED_1, ints[1]); break;
      case 1: digitalWrite(LED_2, ints[1]); break;
      case 2:
        analogWrite(R_PIN, ints[1]);
        analogWrite(G_PIN, ints[2]);
        analogWrite(B_PIN, ints[3]);
        break;
      case 3: servo.write(ints[1]); break;
      }
    */
    /*
        // разделяет строку на подстроки
        char* ptrs[10];       // указатели на строки
        int count = 0;        // счётчик подстрок
        char* offset = str;   // указатель для работы
        while (true) {
          ptrs[count++] = offset;         // запоминаем указатель
          offset = strchr(offset, ',');   // ищем новую запятую
          if (offset) {                   // если это не NULL
             offset = NULL;               // заменяем запятую на нулл
            offset++;                     // продолжаем
          }
          else break;                     // иначе покидаем цикл
        }
        // выводим строки
        for (int i = 0; i < count; i++) Serial.println(ptrs[i]);
    */
    /*
        // достаёт инты из строки и кладёт в int буфер
        int data[10];         // буфер интов
        int count = 0;        // счётчик интов
        char* offset = str;   // указатель для работы
        while (true) {
          data[count++] = atoi(offset); // пишем число в буфер
          offset = strchr(offset, ','); // поиск следующей запятой
          if (offset) offset++;         // если это не NULL - продолжаем
          else break;                   // иначе покидаем цикл
        }
        // выводим инт буфер
        for (int i = 0; i < count; i++) Serial.println(data[i]);
    */
  //}
}
