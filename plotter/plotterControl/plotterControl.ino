void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  Serial.flush();
  Serial.println("sin1 , sin2 , sin3");
}

int degr = 0;
int shift = 0;
int ampl = 0;

void loop() {
  if (Serial.available() > 1) {
    char key = Serial.read();
    int val = Serial.parseInt();
    switch (key) {
      case 'a': ampl = val; break;
      case 's': shift = val; break;
    }
  }
  Serial.print(ampl * sin(radians(degr + shift)));
  Serial.print(',');
  Serial.println(sin(radians(degr)));
  degr++;
}
