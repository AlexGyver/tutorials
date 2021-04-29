// ====================================
// БАЗОВЫЙ ПРИМЕР ДЛЯ ОБЩЕНИЯ С АРДУИНО
// ====================================
int speed = 115200;
String portName;     // имя порта

import processing.serial.*;
Serial serial;
import controlP5.*;
ControlP5 cp5;
boolean skip = true;

import meter.*;
Meter m;
Chart myChart;
int x = 250, y = 100;
int potVal;
int fillVal = 0;

void setup() {
  size(500, 250);    // размер окна
  setupGUI();        // инициализация интерфейса
}

void led(int val) {
  sendPacket(0, str(val));
}
void fan(int val) {
  sendPacket(3, str(val));
}
void bulb(int val) {
  sendPacket(4, str(val));
}
void send() {
  sendPacket(5, cp5.get(Textfield.class, "input").getText());
}
void picker(int col) {  
  String str = str(int(red(col))) + ',';
  str += str(int(green(col))) + ',';
  str += str(int(blue(col)));
  sendPacket(1, str);
}
void knob(int val) {
  sendPacket(2, str(val));
}

void draw() {
  background(120);   // заливаем фон
  parsing();         // парсим
  m.updateMeter(potVal);
  fill(fillVal);
  circle(x, y, 30);

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
      potVal = int(data[1]);
      myChart.push("incoming", int(data[2]));
      cp5.get(Slider.class, "temp").setValue(float(data[3]));
      break;
    case 1:
      fillVal = int(data[1]) * 255;
      break;
    case 2:
      x += map(int(data[1])+5, 0, 1023, -5, 5);
      y += map(int(data[2]), 0, 1023, 5, -5);
      break;
    }
  }
}

// ======= ИНИЦИАЛИЗАЦИЯ ИНТЕРФЕЙСА ========
void setupGUI() {
  cp5 = new ControlP5(this);
  cp5.setFont(createFont("Calibri", 16));  // сделаем шрифт побольше

  // добавляем кнопки
  cp5.addButton("open").setPosition(90, 10).setSize(80, 30);
  cp5.addButton("close").setPosition(170, 10).setSize(80, 30).linebreak();


  // тогглы
  cp5.addToggle("led").setMode(ControlP5.SWITCH);
  cp5.addToggle("fan").setMode(ControlP5.SWITCH);
  cp5.addToggle("bulb").setMode(ControlP5.SWITCH);    

  cp5.addColorWheel("picker", 10, 110, 100);

  myChart = cp5.addChart("light")
    .setPosition(280, 140)
    .setSize(200, 80)
    .setRange(0, 1023)
    .setView(Chart.LINE)
    .addDataSet("incoming")
    .setData("incoming", new float[100]); 
  ;

  cp5.addSlider("temp")
    .setPosition(120, 110)
    .setSize(20, 100)
    .setRange(20, 40)
    ;
  cp5.addKnob("knob").setRange(0, 180).setRadius(30).setPosition(180, 130);


  cp5.addTextfield("input").setPosition(160, 60).setSize(100, 20);
  cp5.addButton("send").setPosition(160, 80).setSize(100, 20);
  // meter
  m = new Meter(this, 280, 10);
  m.setMeterWidth(200);
  m.setUp(0, 1023, 0, 100, -180, 0);
  String[] scaleLabels = {"0", "20", "40", "60", "80", "100"};
  m.setScaleLabels(scaleLabels);


  // выпадающий список
  cp5.addScrollableList("com")
    .setPosition(10, 10)
    .setSize(80, 100)
    .setBarHeight(30)
    .setItemHeight(30)
    .close()
    .addItems(Serial.list());    
  ;
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
