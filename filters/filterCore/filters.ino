const int NUM_READ = 50;  // количество усреднений для средних арифм. фильтров

// обычное среднее арифметическое
float midArifm() {
  float sum = 0;                      // локальная переменная sum
  for (int i = 0; i < NUM_READ; i++)  // согласно количеству усреднений
    sum += getSignal();               // суммируем значения с любого датчика в переменную sum
  return (sum / NUM_READ);
}

// растянутое среднее арифметическое
float midArifm2(float newVal) {
  static byte counter = 0;
  static float prevResult = 0;
  static float sum = 0;
  sum += newVal;
  counter++;
  if (counter == NUM_READ) {
    prevResult = sum / NUM_READ;
    sum = 0;
    counter = 0;
  }
  return prevResult;
}

// бегущее среднее арифметическое вообще неоптимальное
float runMiddleArifmBad(float newVal) {  // принимает новое значение
  static float valArray[NUM_READ];    // массив

  for (int i = 0; i < NUM_READ - 1; i++)
    valArray[i] = valArray[i + 1];

  valArray[NUM_READ - 1] = newVal;    // пишем новое в крайнюю ячейку
  float average = 0;                  // среднее

  for (int i = 0; i < NUM_READ; i++)
    average += valArray[i];           // суммируем

  return (float)average / NUM_READ;   // возвращаем
}

// бегущее среднее арифметическое
float runMiddleArifm(float newVal) {  // принимает новое значение
  static byte idx = 0;                // индекс
  static float valArray[NUM_READ];    // массив
  valArray[idx] = newVal;             // пишем каждый раз в новую ячейку
  if (++idx >= NUM_READ) idx = 0;     // перезаписывая самое старое значение
  float average = 0;                  // обнуляем среднее
  for (int i = 0; i < NUM_READ; i++) {
    average += valArray[i];           // суммируем
  }
  return (float)average / NUM_READ;   // возвращаем
}

// оптимальное бегущее среднее арифметическое
float runMiddleArifmOptim(float newVal) {
  static int t = 0;
  static int vals[NUM_READ];
  static int average = 0;
  if (++t >= NUM_READ) t = 0; // перемотка t
  average -= vals[t];         // вычитаем старое
  average += newVal;          // прибавляем новое
  vals[t] = newVal;           // запоминаем в массив
  return ((float)average / NUM_READ);
}

// бегущее среднее, ещё более оптимальный вариант предыдущего фильтра
float expRunningAverage(float newVal) {
  static float filVal = 0;
  filVal += (newVal - filVal) * 0.2;
  return filVal;
}

// бегущее среднее с адаптивным коэффициентом
float expRunningAverageAdaptive(float newVal) {
  static float filVal = 0;
  float k;
  // резкость фильтра зависит от модуля разности значений
  if (abs(newVal - filVal) > 1.5) k = 0.9;
  else k = 0.03;

  filVal += (newVal - filVal) * k;
  return filVal;
}

// медиана на 3 значения со своим буфером
float median(float newVal) {
  static float buf[3];
  static byte count = 0;
  buf[count] = newVal;
  if (++count > 2) count = 0;

  float a = buf[0];
  float b = buf[1];
  float c = buf[2];

  float middle;
  if ((a <= b) && (a <= c)) {
    middle = (b <= c) ? b : c;
  } else {
    if ((b <= a) && (b <= c)) {
      middle = (a <= c) ? a : c;
    } else {
      middle = (a <= b) ? a : b;
    }
  }
  return middle;
}

// упрощённый калман вроде как
float _err_measure = 0.7;  // примерный шум измерений
float _q = 0.09;   // скорость изменения значений 0.001-1, варьировать самому

float simpleKalman(float newVal) {
  float _kalman_gain, _current_estimate;
  static float _err_estimate = _err_measure;
  static float _last_estimate;

  _kalman_gain = (float)_err_estimate / (_err_estimate + _err_measure);
  _current_estimate = _last_estimate + (float)_kalman_gain * (newVal - _last_estimate);
  _err_estimate =  (1.0 - _kalman_gain) * _err_estimate + fabs(_last_estimate - _current_estimate) * _q;
  _last_estimate = _current_estimate;

  return _current_estimate;
}

// период дискретизации (измерений), process variation, noise variation
float dt = 0.02;
float sigma_process = 3.0;
float sigma_noise = 0.7;

float ABfilter(float newVal) {
  static float xk_1, vk_1, a, b;
  static float xk, vk, rk;
  static float xm;

  float lambda = (float)sigma_process * dt * dt / sigma_noise;
  float r = (4 + lambda - (float)sqrt(8 * lambda + lambda * lambda)) / 4;
  a = (float)1 - r * r;
  b = (float)2 * (2 - a) - 4 * (float)sqrt(1 - a);

  xm = newVal;
  xk = xk_1 + ((float) vk_1 * dt );
  vk = vk_1;
  rk = xm - xk;
  xk += (float)a * rk;
  vk += (float)( b * rk ) / dt;
  xk_1 = xk;
  vk_1 = vk;
  return xk_1;
}
