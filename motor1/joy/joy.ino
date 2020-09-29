#define MOTOR1_IN 2
#define MOTOR1_PWM 3
#define JOY_DEADZONE 10 // "мёртвая зона" джойстика

int joyMiddle;

void setup() {
  pinMode(MOTOR1_IN, OUTPUT);
  pinMode(MOTOR1_PWM, OUTPUT);

  // калибруем ноль джойстика при запуске
  // у китайских он бывает диапазоне 400-600
  joyMiddle = analogRead(0);
}

void loop() {
  motorControl(analogRead(0), MOTOR1_IN, MOTOR1_PWM);
}

// принимает знач. 0-1023, пин IN и PWM
void motorControl(int val, byte pinIN, byte pinPWM) {
  if (val > joyMiddle + JOY_DEADZONE) {
    val = map(val, joyMiddle, 1023, 0, 255);
    analogWrite(pinPWM, val);
    digitalWrite(pinIN, 0);
    
  } else if (val < joyMiddle - JOY_DEADZONE) {
    val = map(val, joyMiddle, 0, 255, 0);
    analogWrite(pinPWM, val);
    digitalWrite(pinIN, 1);
    
  } else {  // стоп    
    digitalWrite(pinIN, 0);
    digitalWrite(pinPWM, 0);
  }
}
