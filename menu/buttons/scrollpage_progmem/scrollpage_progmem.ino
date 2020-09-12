/*
   Пример меню с вертикальным скроллингом при достижении указателем последней строки
*/

#define OLED_SOFT_BUFFER_64   // Буфер на стороне МК
#include <GyverOLED.h>        // Библиотека дисплея
GyverOLED oled;               // Обьект дисплея

#include <GyverButton.h>      // Либа кнопок
GButton up(3, HIGH_PULL);     // Кнопки
GButton down(4, HIGH_PULL);
GButton ok(2, HIGH_PULL);

#define ITEMS 12               // Общее кол во пунктов

// Битмап с картинкой стрелочки (Если нужен)
const uint8_t ptr_bmp[] PROGMEM = {
  0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0xFF, 0xFF, 0x7E, 0x3C, 0x18,
};

/* массивы строк с именами пунктов */
const char i0[] PROGMEM = "  Parameter 0:";  // Каждое имя в своей строке, символ переноса не нужен
const char i1[] PROGMEM = "  Parameter 1:";
const char i2[] PROGMEM = "  Parameter 2:";
const char i3[] PROGMEM = "  Parameter 3:";
const char i4[] PROGMEM = "  Parameter 4:";
const char i5[] PROGMEM = "  Parameter 5:";
const char i6[] PROGMEM = "  Parameter 6:";
const char i7[] PROGMEM = "  Parameter 7:";
const char i8[] PROGMEM = "  Parameter 8:";
const char i9[] PROGMEM = "  Parameter 9:";
const char i10[] PROGMEM = "  Parameter 10:";
const char i11[] PROGMEM = "  Parameter 11:";

const char* const names[] PROGMEM =           // Создаем массив ссылок, обращение к названию пунктов по их номеру
{
  i0, i1, i2, i3,
  i4, i5, i6, i7,
  i8, i9, i10, i11
};

void setup() {
  oled.init(OLED128x64, 500); // Инциализация дисплея
  oled.setContrast(255);      // Макс. яркость
}

void loop() {
  static int8_t pointer = 0; // Переменная указатель

  /* Кнопки */
  up.tick();                 // Опрос кнопок
  down.tick();
  ok.tick();

  if (up.isClick() or up.isHold()) {                // Если кнопку нажали или удерживают
    pointer = constrain(pointer - 1, 0, ITEMS - 1); // Двигаем указатель в пределах дисплея
  }

  if (down.isClick() or down.isHold()) {
    pointer = constrain(pointer + 1, 0, ITEMS - 1);
  }

  if (ok.isClick()) {   // Нажатие на ОК - переход в пункт меню
    switch (pointer) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
      case 0: func(); break;  // По нажатию на ОК при наведении на 0й пункт вызвать функцию
      case 1: break;
      case 2: break;
      case 3: break;
      case 4: break;
      case 5: break;
      case 6: break;
      case 7: break;
    }
  }

  /* меню */
  oled.clear();                     // Очищаем буфер
  oled.home();                      // Курсор в левый верхний угол
  for (uint8_t i = 0; i < 8; i++) { // Цикл, выводящий 8 пунктов на дисплей
    printMenuItem(pointer > 7 ? i + (pointer - 7) : i); // Выводим пункты + скроллинг при достижении последней строки указателем
  }                                                     // Если указатель > 7, то сдвигаем список пунктов на pointer - 7
  printPointer(constrain(pointer, 0, 7));               // Вывод указателя, в пределах 0-7 (не даем переполняться)
  oled.update();                    // Выводим кадр на дисплей
}

void printPointer(uint8_t pointer) {
  // Указатель в начале строки
  oled.setCursor(0, pointer);
  oled.print(">");
  // Можно еще в конце
  /*oled.setCursor(20, pointer);
    oled.print("<");*/
  // Указатель в виде прямоугольной рамки со скругленными углами
  /*oled.roundRect(0, pointer * 8, 127, (pointer * 8) + 6);*/
  // Указатель в виде битмапа (картинки)
  /*oled.drawBitmap(0, pointer * 8, ptr_bmp, 10, 8);*/
}

/* пример вложеной функции, которую можно вызвать из под меню */
void func(void) {
  oled.clear();
  oled.home();
  oled.print(F("Press OK to return"));
  oled.update();
  while (1) {
    ok.tick();
    if (ok.isClick()) return; // return возвращает нас в предыдущее меню
  }
}

// Функция для печати строки из prm
void printMenuItem(uint8_t num) {
  char buffer[21];                                // Буфер на полную строку
  uint16_t ptr = pgm_read_word(&(names[num]));    // Получаем указатель на первый символ строки
  uint8_t i = 0;                                  // Переменная - счетчик

  do {                                            // Начало цикла
    buffer[i] = (char)(pgm_read_byte(ptr++));     // Прочитать в буфер один символ из PGM и подвинуть указатель на 1
  } while (buffer[i++] != NULL);                  // Если это не конец строки - вернуться в начало цикла

  oled.println(buffer);                           // Вывод готовой строки с переносом на следующую
}
