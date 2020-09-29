#define MOTOR1_IN 2
#define MOTOR1_PWM 3
#define MOTOR2_IN 4
#define MOTOR2_PWM 5

void setup() {
  pinMode(MOTOR1_IN, OUTPUT);
  pinMode(MOTOR1_PWM, OUTPUT);
  pinMode(MOTOR2_IN, OUTPUT);
  pinMode(MOTOR2_PWM, OUTPUT);
}

void loop() {
  motorControl(analogRead(0), MOTOR1_IN, MOTOR1_PWM);

  // а второй мотор - в другую сторону!
  motorControl(1023 - analogRead(0), MOTOR2_IN, MOTOR2_PWM);
}

// принимает знач. 0-1023, пин IN и PWM
void motorControl(int val, byte pinIN, byte pinPWM) {
  val = map(val, 0, 1023, -255, 255);

  if (val > 0) {  // вперёд
    analogWrite(pinPWM, val);
    digitalWrite(pinIN, 0);
  } else if (val < 0) {  // назад
    analogWrite(pinPWM, 255 + val);
    digitalWrite(pinIN, 1);
  } else {  // стоп
    digitalWrite(pinIN, 0);
    digitalWrite(pinPWM, 0);
  }
}
