// пример с оптимизированным линейным меню
// оптимизировано очищение дисплея
// названия засунуты в PROGMEM вместо оперативки
// менять значения можно нажатым поворотом, или КЛИКНУТЬ, курсор изменится, и крутить
// значение меняется быстрее при быстром повороте

#define LINES 4       // количество строк дисплея
#define SETTINGS_AMOUNT 7  // количество настроек
#define FAST_STEP 5   // скорость изменения при быстром повороте

// пины энкодера
#define CLK 2
#define DT 3
#define SW 4

#include "GyverEncoder.h"
Encoder enc(CLK, DT, SW);  // для работы c кнопкой

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // адрес 0x27 или 0x3f

int vals[SETTINGS_AMOUNT];  // массив параметров
int8_t arrowPos = 0;
bool controlState = 0;  // клик

// названия параметров
const char name1[] PROGMEM = "Speed";
const char name2[] PROGMEM = "Temp";
const char name3[] PROGMEM = "Mode";
const char name4[] PROGMEM = "Direction";
const char name5[] PROGMEM = "Limit";
const char name6[] PROGMEM = "Color";
const char name7[] PROGMEM = "Timeout";

// объявляем таблицу ссылок
const char* const names[] PROGMEM = {
  name1, name2, name3, name4, name5, name6, name7,
};

void setup() {
  //Serial.begin(9600);
  enc.setType(TYPE2);
  enc.setFastTimeout(100);

  lcd.init();
  lcd.backlight();
  printGUI();
}

void loop() {
  enc.tick();

  // смена контроля
  if (enc.isClick()) {
    controlState = !controlState;
    printGUI();
  }

  if (enc.isTurn()) {
    int increment = 0;  // локальная переменная направления

    // получаем направление
    if (!controlState) {
      if (enc.isRight()) increment = 1;
      if (enc.isLeft()) increment = -1;
      arrowPos += increment;  // двигаем курсор
      arrowPos = constrain(arrowPos, 0, SETTINGS_AMOUNT - 1); // ограничиваем
    }

    increment = 0;  // обнуляем инкремент
    if ((controlState && enc.isRight()) || enc.isRightH()) increment = 1;
    if ((controlState && enc.isLeft()) || enc.isLeftH()) increment = -1;
    
    if (controlState && enc.isFastR()) increment = FAST_STEP;
    if (controlState && enc.isFastL()) increment = -FAST_STEP;
    vals[arrowPos] += increment;  // меняем параметры

    printGUI();
  }
}

void printGUI() {
  static int8_t screenPos = 0; // номер "экрана"
  static int8_t lastScreen = 0; // предыдущий номер "экрана"

  screenPos = arrowPos / LINES;   // ищем номер экрана (0..3 - 0, 4..7 - 1)
  if (lastScreen != screenPos) lcd.clear(); // если экран сменился - очищаем
  lastScreen = screenPos;

  for (byte i = 0; i < LINES; i++) {  // для всех строк
    lcd.setCursor(0, i);              // курсор в начало

    // если курсор находится на выбранной строке
    smartArrow(arrowPos == LINES * screenPos + i);  // рисуем стрелку или пробел

    // если пункты меню закончились, покидаем цикл for
    if (LINES * screenPos + i == SETTINGS_AMOUNT) break;

    // выводим имя и значение пункта меню
    printFromPGM(&names[LINES * screenPos + i]);
    lcd.print(F(": "));
    lcd.print(vals[LINES * screenPos + i]);
    lcd.print(F("  ")); // пробелы для очистки
  }
}

// очень хитрая функция для печати из PROGMEM
void printFromPGM(int charMap) {
  uint16_t ptr = pgm_read_word(charMap);    // получаем адрес из таблицы ссылок
  while (pgm_read_byte(ptr) != NULL) {      // всю строку до нулевого символа
    lcd.print(char(pgm_read_byte(ptr)));    // выводим в монитор или куда нам надо
    ptr++;                                  // следующий символ
  }
}

void smartArrow(bool state) {  // рисует стрелку, галку или пробел
  lcd.write(state ? (controlState ? 62 : 126) : 32);
}
