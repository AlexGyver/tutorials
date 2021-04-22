// управляем второй ардуиной
#include <GyverButton.h>
GButton btn(3);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // кнопка
  btn.tick();
  if (btn.isClick()) {
    static bool flag = 0;
    flag = !flag;
    Serial.print("0,");
    Serial.print(flag);
    Serial.print(';');
  }
  
  // таймер
  static uint32_t tmr = 0;
  if (millis() - tmr > 50) {  // 20 раз в секунду
    tmr = millis();
    Serial.print("3,");
    int val = analogRead(0);
    val = map(val, 0, 1023, 0, 180);  // 0..1023 - 0..180
    Serial.print(val);
    Serial.print(';');
  }

}
