/*
   Пример одностраничного меню с прямой регулировкой параметров (F macro)
*/

#define OLED_SOFT_BUFFER_64     // Буфер на стороне МК
#include <GyverOLED.h>          // Библиотека дисплея
GyverOLED<SSH1106_128x64> oled; // Обьект дисплея

#include <GyverButton.h>      // Либа кнопок
GButton up(3, HIGH_PULL);     // Кнопки
GButton down(4, HIGH_PULL);
GButton ok(2, HIGH_PULL);

#define ITEMS 8               // Общее кол во пунктов

// Битмап с картинкой стрелочки (Если нужен)
const uint8_t ptr_bmp[] PROGMEM = {
  0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0xFF, 0xFF, 0x7E, 0x3C, 0x18,
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
  oled.clear();           // Очищаем буфер
  oled.home();            // Курсор в левый верхний угол
  oled.print              // Вывод всех пунктов
  (F(
     "  Parameter 0:\n"   // Не забываем про '\n' - символ переноса строки
     "  Parameter 1:\n"
     "  Parameter 2:\n"
     "  Parameter 3:\n"
     "  Parameter 4:\n"
     "  Parameter 5:\n"
     "  Parameter 6:\n"
     "  Parameter 7:\n"
   ));

  for (uint8_t i = 0; i < 8; i++) { // Цикл для вывода параметров (8 строк)
    oled.setCursor(14, i);          // Ставим курсор на позицию 14го сивола и номер строки
    oled.print(data[i]);            // Выводим значение параметра
  }

  printPointer(pointer);  // Вывод указателя
  oled.update();          // Выводим кадр на дисплей
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
