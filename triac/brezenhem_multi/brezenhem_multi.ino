// пример диммера на симисторе с внешним детектором нуля
// или готовый китайский модуль https://ali.ski/xwpCCe
// многоканальный  диммер без таймера, работает на пропуске
// полуволн синусоиды по прерываниям с детектора нуля
// подходит для моторов и обогревателей, лампы мерцают жутко!
// Алгоритм Брезенхема https://habr.com/ru/post/254719/
// диммер принимает значения 0-255
// включения выключения оптимизированы
#define ZERO_PIN 2    // пин детектора нуля
#define INT_NUM 0     // соответствующий ему номер прерывания
#define DIM_AMOUNT 3  // количество диммеров

const byte dimPins[] = {3, 4, 5}; // их пины
int dimmer[DIM_AMOUNT]; // переменная диммеров

void setup() {
  pinMode(ZERO_PIN, INPUT_PULLUP);
  for (byte i = 0; i < DIM_AMOUNT; i++) pinMode(dimPins[i], OUTPUT);  
  attachInterrupt(INT_NUM, isr, FALLING);  // для самодельной схемы ставь RISING!
}

void loop() {
  // задаём значение 0-255
  dimmer[0] = 50;
  dimmer[1] = 120;
  dimmer[2] = 190;
  delay(100); // в реальном коде задержек быть не должно
}

// прерывание детектора нуля
void isr() {
  static byte count, last[DIM_AMOUNT], lastState[DIM_AMOUNT];
  count++;
  for (byte i = 0; i < DIM_AMOUNT; i++) {
    int val = (count * dimmer[i]) >> 8;
    if (lastState[i] != (val != last[i])) digitalWrite(dimPins[i], val != last[i]);
    lastState[i] = (val != last[i]);
    last[i] = val;
  }
}
