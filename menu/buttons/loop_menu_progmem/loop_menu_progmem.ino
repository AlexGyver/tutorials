/*
   Пример меню со статическим указателем (перемещаются сами пукты меню)
*/

#define OLED_SOFT_BUFFER_64     // Буфер на стороне МК
#include <GyverOLED.h>          // Библиотека дисплея
GyverOLED<SSH1106_128x64> oled; // Обьект дисплея

#include <GyverButton.h>      // Либа кнопок
GButton up(3, HIGH_PULL);     // Кнопки
GButton down(4, HIGH_PULL);
GButton ok(2, HIGH_PULL);

#define ITEMS 12               // Общее кол во пунктов

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
  oled.init();
  oled.setContrast(255);
}

void loop() {
  static int8_t pointer = 0; // Переменная указатель

  /* Кнопки */
  up.tick();                 // Опрос кнопок
  down.tick();
  ok.tick();

  if (up.isClick() or up.isHold()) {          // Нажатие или удержание вверх
    if (--pointer < 1) pointer = ITEMS - 1;   // Уменьшаем указатель на 1 и если он стал меньше 1 -> присваиваем указателю ITEMS - 1
  }

  if (down.isClick() or down.isHold()) {      // Нажатие или удержание вниз
    if (++pointer > ITEMS - 1) pointer = 0;   // Увеличиваем указатель на 1 и если он стал больше ITEMS - 1 -> присваиваем указателю 0
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
        // И остальные ...
    }
  }

  oled.clear();           // Очищаем буфер
  oled.home();            // Курсор в левый верхний угол

  for (uint8_t i = pointer; i <  pointer + 8; i++) { // Цикл считающий от значения указателя еще 8 раз
    printMenuItem(i > ITEMS - 1 ? i - ITEMS : i);    // Если результат больше ITEMS - 1 -> начинаем сначала, иначе -> продолжаем как обычно
  }

  /* Указатель стоит на месте */
  oled.setCursor(0, 0);
  oled.print(">");
  oled.setCursor(20, 0);
  oled.print("<");

  oled.update();
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
