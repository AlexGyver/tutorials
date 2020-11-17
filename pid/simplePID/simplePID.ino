// функция ПИД регулятора
void setup() {
}

void loop() {
  //analogWrite(pin, computePID(sensorRead, 30, 1.0, 2.0, 3.0, 0.02));
  //delay(20);
}

// (вход, установка, п, и, д, период в секундах)
int computePID(float input, float setpoint, float kp, float ki, float kd, float dt) {
  float err = setpoint - input;
  static float integral = 0, prevErr = 0;
  integral += err * dt;
  float D = (err - prevErr) / dt;
  prevErr = err;
  return (err * kp + integral * ki + D * kd);
}
