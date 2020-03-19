// пины энкодера
#define CLK 2
#define DT 3
#define SW 4

#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);  // для работы c кнопкой

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // адрес 0x27 или 0x3f


int vals[4] = {0, 0, 0, 0}; // массив параметров
int8_t arrowPos = 0;  // позиция стрелки

void setup() {
  Serial.begin(9600);
  enc1.setType(TYPE2);

  lcd.init();
  lcd.backlight();
  printGUI();   // выводим интерфейс
}

void loop() {
  enc1.tick();

  if (enc1.isTurn()) {  // при любом повороте
    lcd.clear();
    int increment = 0;  // локальная переменная направления
    
    // получаем направление    
    if (enc1.isRight()) increment = 1;
    if (enc1.isLeft()) increment = -1;
    
    arrowPos += increment;  // двигаем курсор   
    arrowPos = constrain(arrowPos, 0, 3); // ограничиваем

    increment = 0;  // обнуляем инкремент
    if (enc1.isRightH()) increment = 1;
    if (enc1.isLeftH()) increment = -1;
    // меняем параметры
    vals[arrowPos] += increment;
    
    printGUI();
  }
}

void printGUI() {
  lcd.setCursor(0, 0); lcd.print("val1:"); lcd.print(vals[0]);
  lcd.setCursor(8, 0); lcd.print("val2:"); lcd.print(vals[1]);
  lcd.setCursor(0, 1); lcd.print("val3:"); lcd.print(vals[2]);
  lcd.setCursor(8, 1); lcd.print("val4:"); lcd.print(vals[3]);
  // выводим стрелку
  switch (arrowPos) {
    case 0: lcd.setCursor(4, 0);
      break;
    case 1: lcd.setCursor(12, 0);
      break;
    case 2: lcd.setCursor(4, 1);
      break;
    case 3: lcd.setCursor(12, 1);
      break;
  }
  lcd.write(126);   // вывести стрелку
}
