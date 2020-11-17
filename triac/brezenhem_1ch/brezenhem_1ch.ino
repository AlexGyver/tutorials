// пример диммера на симисторе с внешним детектором нуля
// или готовый китайский модуль https://ali.ski/xwpCCe
// одноканальный диммер без таймера, работает на пропуске
// полуволн синусоиды по прерываниям с детектора нуля
// подходит для моторов и обогревателей, лампы мерцают жутко!
// Алгоритм Брезенхема https://habr.com/ru/post/254719/
// диммер принимает значения 0-255
// переключения оптимизированы
#define ZERO_PIN 2    // пин детектора нуля
#define INT_NUM 0     // соответствующий ему номер прерывания
#define DIMMER_PIN 4  // управляющий пин симистора

int dimmer;           // переменная диммера

void setup() {
  pinMode(ZERO_PIN, INPUT_PULLUP);
  pinMode(DIMMER_PIN, OUTPUT);
  digitalWrite(DIMMER_PIN, LOW);
  attachInterrupt(INT_NUM, isr, FALLING);  // для самодельной схемы ставь RISING!
}

void loop() {
  // задаём значение 0-255
  dimmer = map(analogRead(A0), 0, 1023, 0, 255);
  delay(100); // в реальном коде задержек быть не должно
}

// прерывание детектора нуля
void isr() {
  static byte count, last, lastVal;
  int val = (++count * dimmer) >> 8;
  if (lastVal != (val != last)) digitalWrite(DIMMER_PIN, val != last);
  lastVal = (val != last);
  last = val;
}
