#define SW 0
#define DT 3
#define CLK 2
#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);  // для работы c кнопкой

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 20, 4);

int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;
int8_t arrowPos = 0;

void setup() {
  Serial.begin(9600);
  enc1.setType(TYPE2);

  lcd.init();
  lcd.backlight();
  printGUI();
}

void loop() {
  enc1.tick();

  if (enc1.isTurn()) {
    lcd.clear();

    if (enc1.isRight()) {
      arrowPos++;
      if (arrowPos >= 4) arrowPos = 3;
    }
    if (enc1.isLeft()) {
      arrowPos--;
      if (arrowPos < 0) arrowPos = 0;
    }

    if (enc1.isRightH()) {
      switch (arrowPos) {
        case 0: val1++;
          break;
        case 1: val2++;
          break;
        case 2: val3++;
          break;
        case 3: val4++;
          break;
      }
    }
    if (enc1.isLeftH()) {
      switch (arrowPos) {
        case 0: val1--;
          break;
        case 1: val2--;
          break;
        case 2: val3--;
          break;
        case 3: val4--;
          break;
      }
    }
    printGUI();
  }
}

void printGUI() {
  lcd.setCursor(0, 0); lcd.print("val1:"); lcd.print(val1);
  lcd.setCursor(8, 0); lcd.print("val2:"); lcd.print(val2);
  lcd.setCursor(0, 1); lcd.print("val3:"); lcd.print(val3);
  lcd.setCursor(8, 1); lcd.print("val4:"); lcd.print(val4);
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
  lcd.write(126);
}
