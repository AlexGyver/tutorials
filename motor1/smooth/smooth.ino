#define MOTOR1_IN 2
#define MOTOR1_PWM 3
#define JOY_DEADZONE 10 // "мёртвая зона" джойстика

int step = 15; // шаг изменения скорости
int joyMiddle;

void setup() {
  pinMode(MOTOR1_IN, OUTPUT);
  pinMode(MOTOR1_PWM, OUTPUT);

  // калибруем ноль джойстика при запуске
  // у китайских он бывает диапазоне 400-600
  joyMiddle = analogRead(0);
}

void loop() {
  smoothControl(analogRead(0));   // плавное управление
}

void smoothControl(int speed) {
  static uint32_t tmr = 0;
  static int lastSpeed = 512; // начальное значение

  if (millis() - tmr >= 50) {  // каждые 50 мс
    tmr = millis();
    // если разница текущей и установленной больше шага изменения
    if (abs(lastSpeed - speed) > step) {  
      lastSpeed += (lastSpeed < speed) ? step : -step;  // прибавлем или вычитаем
    } else {  // иначе
      lastSpeed = speed;  // новая скорость равна заданной
    }
    motorControl(lastSpeed, MOTOR1_IN, MOTOR1_PWM);
  }
}

// принимает знач. 0-1023, пин IN и PWM
void motorControl(int val, byte pinIN, byte pinPWM) {
  // вперёд - значение больше середины
  if (val > joyMiddle + JOY_DEADZONE) {
    val = map(val, joyMiddle, 1023, 0, 255);
    analogWrite(pinPWM, val);
    digitalWrite(pinIN, 0);

    // назад - значение меньше середины
  } else if (val < joyMiddle - JOY_DEADZONE) {
    val = map(val, joyMiddle, 0, 255, 0);
    analogWrite(pinPWM, val);
    digitalWrite(pinIN, 1);

    // стоп - мы в мёртвой зоне
  } else {
    digitalWrite(pinIN, 0);
    digitalWrite(pinPWM, 0);
  }
}
