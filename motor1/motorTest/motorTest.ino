#define MOTOR_PIN_A 2
#define MOTOR_PIN_B 4
#define MOTOR_PIN_PWM 3

void setup() {
  pinMode(MOTOR_PIN_A, OUTPUT);
  pinMode(MOTOR_PIN_B, OUTPUT);
  pinMode(MOTOR_PIN_PWM, OUTPUT);
  // Пины D3 и D11 - 31.4 кГц
  TCCR2B = 0b00000001;  // x1
  TCCR2A = 0b00000001;  // phase correct
}

void loop() {
  motorControl(analogRead(0));
}

void motorControl(int val) {
  val = map(val, 0, 1023, -255, 255);
  if (val > 0) {
    analogWrite(MOTOR_PIN_PWM, val);
    digitalWrite(MOTOR_PIN_A, 1);
    digitalWrite(MOTOR_PIN_B, 0);
  } else if (val < 0) {
    analogWrite(MOTOR_PIN_PWM, -val);
    digitalWrite(MOTOR_PIN_A, 0);
    digitalWrite(MOTOR_PIN_B, 1);
  } else {
    analogWrite(MOTOR_PIN_PWM, 0);
    digitalWrite(MOTOR_PIN_A, 1);
    digitalWrite(MOTOR_PIN_B, 1);
  }
}
