int speed = 115200;
import meter.*;
Meter m;

import processing.serial.*;
Serial serial;
import controlP5.*;
ControlP5 cp5;
String portName;
int potVal;
int fillVal = 0;
Chart myChart;
int x = 250, y = 100;

void setup() {
  size(500, 250);
  cp5 = new ControlP5(this);
  cp5.setFont(createFont("Calibri", 16));  // сделаем шрифт побольше

  // кнопки
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


  // выпадающий список
  cp5.addScrollableList("com")
    .setPosition(10, 10)
    .setSize(80, 100)
    .setBarHeight(30)
    .setItemHeight(30)
    .close()
    .addItems(Serial.list());    // добавляем список портов
  ;

  // meter
  m = new Meter(this, 280, 10);
  m.setMeterWidth(200);
  m.setUp(0, 1023, 0, 100, -180, 0);
  String[] scaleLabels = {"0", "20", "40", "60", "80", "100"};
  m.setScaleLabels(scaleLabels);
}

void com(int n) {
  portName = Serial.list()[n];
}
void open() {
  serial = new Serial(this, portName, speed);
}
void close() {
  serial.stop();
}

void led(int val) {
  serial.write("0,"+ val +";");
}
void fan(int val) {
  serial.write("3,"+ val +";");
}
void bulb(int val) {
  serial.write("4,"+ val +";");
}
void send() {
  serial.write("5,"+ cp5.get(Textfield.class, "input").getText() +";");
}


void picker(int col) {
  String str = "1,";
  str += int(red(col));
  str += ',';
  str += int(green(col));
  str += ',';
  str += int(blue(col));
  str += ';';
  if (serial != null) serial.write(str);
}
void knob(int val) {
  String str = "2," + val + ';';
  if (serial != null) serial.write(str);
}


void draw() {
  background(120);
  m.updateMeter(potVal);
  fill(fillVal);
  circle(x, y, 30);

  //println(mouseX, mouseY);

  if (serial != null) {
    if (serial.available() > 0) {
      String str = serial.readStringUntil('\n');
      str = str.trim();
      String data[] = str.split(",");
      switch (int(data[0])) {
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
}
