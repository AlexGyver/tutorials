// функция ПИД регулятора
// добавил ограничения
void setup() {
}

void loop() {
  //analogWrite(pin, computePID(sensorRead, 30, 1.0, 2.0, 3.0, 0.02, 0, 255));
  //delay(20);
}

// (вход, установка, п, и, д, период в секундах, мин.выход, макс. выход)
int computePID(float input, float setpoint, float kp, float ki, float kd, float dt, int minOut, int maxOut) {
  float err = setpoint - input;
  static float integral = 0, prevErr = 0;
  integral = constrain(integral + (float)err * dt * ki, minOut, maxOut);
  float D = (err - prevErr) / dt;
  prevErr = err;
  return constrain(err * kp + integral + D * kd, minOut, maxOut);
}
