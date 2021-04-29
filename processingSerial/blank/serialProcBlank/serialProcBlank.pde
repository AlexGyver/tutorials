// ====================================
// БАЗОВЫЙ ПРИМЕР ДЛЯ ОБЩЕНИЯ С АРДУИНО
// ====================================
int speed = 115200;

import processing.serial.*;
Serial serial;
import controlP5.*;
ControlP5 cp5;
String portName;     // имя порта
boolean skip = true;

void setup() {
  size(400, 400);    // размер окна
  setupGUI();        // инициализация интерфейса
}

void draw() {
  background(120);   // заливаем фон
  parsing();         // парсим

  // для удобной отправки пакетов можно юзать sendPacket(ключ,"дата1,дата2,дата3") 
  // например sendPacket(5, "255,255,255") - отправить три значения по ключу 5
}


// функция для отправки пакета на арду
void sendPacket(int key, String data) {
  if (serial != null) serial.write(str(key) + ',' + data + ';');
  // отправит "key,data;"
}


// функция парсинга, опрашивать в лупе
void parsing() {
  // если порт открыт и в буфере что то есть
  if (serial != null && serial.available() > 0) {
    String str = serial.readStringUntil('\n').trim();  // читаем строку до \n и подрезаем
    String data[] = str.split(",");                    // разбиваем строку
    //int[] data = int(split(str, ','));  // парсить можно сразу в int[]!
    if (skip) {
      skip = false;
      return;  // пропускаем первый пакет
    }


    switch (int(data[0])) {  // свитч по ключу
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    }
  }
}

// ======= ИНИЦИАЛИЗАЦИЯ ИНТЕРФЕЙСА ========
void setupGUI() {
  cp5 = new ControlP5(this);
  cp5.setFont(createFont("Calibri", 16));  // сделаем шрифт побольше

  // выпадающий список
  cp5.addScrollableList("com")
    .setPosition(10, 10)
    .setSize(80, 100)
    .setBarHeight(30)
    .setItemHeight(30)
    .close()
    .addItems(Serial.list());    
  ;

  // добавляем кнопки
  cp5.addButton("open").setPosition(90, 10).setSize(80, 30);
  cp5.addButton("close").setPosition(170, 10).setSize(80, 30);
}

// ==== ОБРАБОТЧИКИ ИНТЕРФЕЙСА =====
// список портов
void com(int n) {
  portName = Serial.list()[n];  // запоминаем выбранный порт в portName
}

// кнопка открыть порт
void open() {
  if (portName != null && serial == null) {     // если выбран порт и сейчас он закрыт
    serial = new Serial(this, portName, speed); // открываем portName
    skip = true;    // флаг на пропуск первого пакета
  }
}

// кнопка закрыть порт
void close() {
  if (serial != null) { // если порт открыт
    serial.stop();      // закрываем portName
    serial = null;      // serial выключен
  }
}
