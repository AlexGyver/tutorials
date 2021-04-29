// =======================================
// БАЗОВЫЙ ПРИМЕР ДЛЯ ОБЩЕНИЯ С ПРОЦЕССИНГ
// =======================================
#include "Parser.h"       // библиотека парсера
#include "AsyncStream.h"  // асинхронное чтение сериал
AsyncStream<50> serial(&Serial, ';');   // указываем обработчик и стоп символ

void setup() {
  Serial.begin(115200);
}

void loop() {
  parsing();

  // для отправки нескольких интов используй
  /*
    int packet[3];
    packet[0] = 255;
    packet[1] = 255;
    packet[2] = 255;
    sendPacket(0, packet, 3);
  */
}

// функция для отправки пакета на ПК
void sendPacket(int key, int* data, int amount) {
  Serial.print(key);
  Serial.print(',');
  for (int i = 0; i < amount; i++) {
    Serial.print(data[i]);
    if (i != amount - 1) Serial.print(',');
  }
  Serial.print('\n');
}

// функция парсинга, опрашивать в лупе
void parsing() {
  if (serial.available()) {
    Parser data(serial.buf, ',');  // отдаём парсеру
    int ints[10];           // массив для численных данных
    data.parseInts(ints);   // парсим в него

    switch (ints[0]) {      // свитч по ключу
      case 0:
        break;
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;
    }
  }
}
