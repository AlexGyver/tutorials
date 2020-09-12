// пример с линейным меню
#define LINES 4   // количество строк дисплея
#define SETTINGS_AMOUNT 10  // количество настроек

// пины энкодера
#define CLK 2
#define DT 3
#define SW 4

#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);  // для работы c кнопкой

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // адрес 0x27 или 0x3f

int vals[SETTINGS_AMOUNT];  // массив параметров
int8_t arrowPos = 0;
int8_t screenPos = 0; // номер "экрана"

// названия параметров
const char *settingsNames[]  = {
  "Speed",
  "Temp",
  "Mode",
  "Direction",
  "Limit",
  "Color",
  "Timeout",
  "Light",
  "Parameter 1",
  "Parameter 2",
};

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
    int increment = 0;  // локальная переменная направления
    
    // получаем направление   
    if (enc1.isRight()) increment = 1;
    if (enc1.isLeft()) increment = -1;
    arrowPos += increment;  // двигаем курсор  
    arrowPos = constrain(arrowPos, 0, SETTINGS_AMOUNT - 1); // ограничиваем

    increment = 0;  // обнуляем инкремент
    if (enc1.isRightH()) increment = 1;
    if (enc1.isLeftH()) increment = -1;
    vals[arrowPos] += increment;  // меняем параметры

    printGUI();
  }
}

void printGUI() {
  lcd.clear();  
  screenPos = arrowPos / LINES;   // ищем номер экрана (0..3 - 0, 4..7 - 1)

  for (byte i = 0; i < LINES; i++) {  // для всех строк
    lcd.setCursor(0, i);              // курсор в начало

    // если курсор находится на выбранной строке
    if (arrowPos == LINES * screenPos + i) lcd.write(126);  // рисуем стрелку
    else lcd.write(32);     // рисуем пробел

    // если пункты меню закончились, покидаем цикл for
    if (LINES * screenPos + i == SETTINGS_AMOUNT) break;

    // выводим имя и значение пункта меню
    lcd.print(settingsNames[LINES * screenPos + i]);
    lcd.print(": ");
    lcd.print(vals[LINES * screenPos + i]);
  }
}
