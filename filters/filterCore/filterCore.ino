int count = 0;            // счётчик процесса
bool flag = 0;
float cleanSignal = 0.0;  // чистый сигнал
float noiseSignal = 0.0;  // сигнал + шум
float filtSignal = 0.0;   // фильтрованный сигнал

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.flush();
  Serial.println("clean, noise, filter");
  delay(100);
}

void loop() {
  process();
  measure();
  Serial.print(cleanSignal);
  Serial.print(',');
  Serial.print(noiseSignal);
  Serial.print(',');
  Serial.println(filtSignal);
  count++;
}

// измерение с заданным периодом
void measure() {
  static uint32_t tmr;
  if (millis() - tmr >= 5) {
    tmr = millis();
    // ВЫБЕРИ СВОЙ ФИЛЬТР!
    //filtSignal = midArifm();
    //filtSignal = midArifm2(getSignal());
    //filtSignal = runMiddleArifmBad(getSignal());
    //filtSignal = runMiddleArifm(getSignal());
    //filtSignal = runMiddleArifmOptim(getSignal());
    //filtSignal = expRunningAverage(getSignal());
    //filtSignal = expRunningAverageAdaptive(getSignal());
    //filtSignal = median(getSignal());
    //filtSignal = expRunningAverage(filtSignal); // + к медиане
    //filtSignal = simpleKalman(getSignal());
    //filtSignal = ABfilter(getSignal());
  }
}

// имитация сигнала с датчика
void process() {
  // синусоида
  //cleanSignal = 4.0 * sin(0.5 * radians(count));

  // квадрат
  if (count % 100 == 0) flag = !flag; // каждые 100 тиков инвертируем сигнал
  cleanSignal = flag * 3;  // делаем квадрат с амплитудой 3
}

float getSignal() {
  noiseSignal = cleanSignal;

  // постоянный шум
  noiseSignal += (random(-8, 9) / 10.0);

  // случайные выбросы
  if (!random(30)) noiseSignal += random(-5, 6);

  return noiseSignal;
}
