/*
   Пример простого одностраничного меню (F macro)
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
  printPointer(pointer);  // Вывод указателя
  oled.update();          // Выводим кадр на дисплей
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
