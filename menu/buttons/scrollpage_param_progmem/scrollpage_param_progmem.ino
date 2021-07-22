/*
   Пример меню с вертикальным скроллингом при достижении указателем последней строки и прямым управлением параметрами
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

uint8_t data[ITEMS]; // Массив для параметров
bool flag = true;    // Флаг выбора

void setup() {
  oled.init();           // Инциализация дисплея
  oled.setContrast(255); // Макс. яркость
}

void loop() {
  static int8_t pointer = 0; // Переменная указатель

  /* Кнопки */
  up.tick();                 // Опрос кнопок
  down.tick();
  ok.tick();

  if (up.isClick() or up.isHold()) {                  // Если кнопку нажали или удерживают
    if (flag) {                                       // Если флаг установлен
      pointer = constrain(pointer - 1, 0, ITEMS - 1); // Двигаем указатель в пределах дисплея
    } else {                                          // Иначе
      data[pointer]++;                                // Увеличиваем параметр на 1
    }
  }

  if (down.isClick() or down.isHold()) {
    if (flag) {                                       // Если флаг установлен
      pointer = constrain(pointer + 1, 0, ITEMS - 1); // Двигаем указатель в пределах дисплея
    } else {                                          // Иначе
      data[pointer]--;                                // Уменьшаем параметр на 1
    }
  }

  if (ok.isClick()) {   // Нажатие на ОК - переключение цели параметр/значение параметра
    flag = !flag;       // Инвертируем флаг
  }


  /* меню */
  oled.clear();                     // Очищаем буфер
  oled.home();                      // Курсор в левый верхний угол

  for (uint8_t i = 0; i < 8; i++) { // Цикл, выводящий 8 пунктов на дисплей
    printMenuItem(pointer > 7 ? i + (pointer - 7) : i); // Выводим пункты + скроллинг при достижении последней строки указателем
  }                                                     // Если указатель > 7, то сдвигаем список пунктов на pointer - 7

  for (uint8_t i = 0; i < 8; i++) {                     // Выводим параметры в цикле
    oled.setCursor(15, i);                              // Ставим курсор на нужную строку и позицию
    oled.print(data[pointer > 7 ? i + (pointer - 7) : i]); // Условие для вывода аналогично строкам
  }

  printPointer(constrain(pointer, 0, 7));               // Вывод указателя, в пределах 0-7 (не даем переполняться)
  oled.update();                    // Выводим кадр на дисплей
}

void printPointer(uint8_t pointer) {
  if (flag) {                      // Если флаг установлен
    oled.setCursor(0, pointer);    // Указываем на параметр
    oled.print(">");
  } else {                         // Иначе
    oled.setCursor(20, pointer);   // Указываем на значение параметра
    oled.print("<");
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
