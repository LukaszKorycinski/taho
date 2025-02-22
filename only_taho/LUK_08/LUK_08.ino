int PRZEBIEG = 5;
int KIERUNEK_0 = 4;
int KIERUNEK_1 = 9;
int ENABLE_TAHO = 11;
int ENABLE_SPEEDO = 10;
int SPEEDO_SIGNAL_IN = 2;  //hall kmh
int TAHO_SIGNAL_IN = 3;    //hall rpm
//13 wbudowana dioda
unsigned long loopTime = 0;

void setup() {
  pinMode(PRZEBIEG, OUTPUT);
  pinMode(ENABLE_TAHO, OUTPUT);
  pinMode(KIERUNEK_0, OUTPUT);
  pinMode(KIERUNEK_1, OUTPUT);
  pinMode(ENABLE_SPEEDO, OUTPUT);
  pinMode(TAHO_SIGNAL_IN, INPUT);
  pinMode(SPEEDO_SIGNAL_IN, INPUT);

  digitalWrite(PRZEBIEG, LOW);
  digitalWrite(ENABLE_TAHO, LOW);
  digitalWrite(KIERUNEK_0, LOW);
  digitalWrite(KIERUNEK_1, HIGH);
  digitalWrite(ENABLE_SPEEDO, LOW);

  attachInterrupt(digitalPinToInterrupt(SPEEDO_SIGNAL_IN), przerwanieSpeedo, RISING);  //Przerwanie na pinie 2
  attachInterrupt(digitalPinToInterrupt(TAHO_SIGNAL_IN), przerwanieTaho, RISING);      //Przerwanie na pinie 3

  test();
  loopTime = millis();
}

void test(){
  loopTime = millis();
  while (loopTime + 1000 > millis()) {
    analogWrite(ENABLE_SPEEDO, calculateSpeedoGaugeValue(100.0f));
    analogWrite(ENABLE_TAHO, calculateRpmGaugeValue(5500.0f));
  }
  while (loopTime + 2000 > millis()) {
    analogWrite(ENABLE_SPEEDO, calculateSpeedoGaugeValue(200.0f));
    analogWrite(ENABLE_TAHO, calculateRpmGaugeValue(11000.0f));
  }
  analogWrite(ENABLE_SPEEDO, 0);
  analogWrite(ENABLE_TAHO, 0);
}

volatile int speedoCounter = 0;
volatile int tahoCounter = 0;

void przerwanieSpeedo() {
  speedoCounter++;
}

void przerwanieTaho() {
  tahoCounter++;
}



void calculateValues() {
  if (loopTime + 1000 < millis()) {
    loopTime = millis();
    float obrNaMinTaho = tahoCounter * 60.0f;
    float kilometersPerHour = speedoCounter * 7.2f;

    tahoCounter = 0;
    speedoCounter = 0;

    analogWrite(ENABLE_SPEEDO, calculateSpeedoGaugeValue(kilometersPerHour));
    analogWrite(ENABLE_TAHO, calculateRpmGaugeValue(obrNaMinTaho));
  }
}


int rpmArraySize = 12;
int rpmArray[12] = { 
  0, //0
  0, //20
  62, //40
  219, //60
  1674, //80
  3437, //100
  5125, //120
  7000, ////140 
  9000, //160
  10875, //180
  12300, //200
  13050, //220
  };
float calculateRpmGaugeValue(float rpm) {

  int lastSmallerIndex = rpmArraySize - 1;

  for (int i = 0; i < rpmArraySize - 2; i++) {
    if (rpm >= rpmArray[i] && rpm < rpmArray[i + 1]) {
      lastSmallerIndex = i;
      break;
    }
  }

  int smaller = rpmArray[lastSmallerIndex];
  int bigger = rpmArray[lastSmallerIndex + 1];
  float diffrence = bigger - smaller;
  float interpolationWave = (rpm - smaller) / diffrence;

  return interpolateF(lastSmallerIndex, lastSmallerIndex+1, interpolationWave) * 20.0f;
}

int kmhArraySize = 12;
int kmhArray[12] = { 
  0, //0
  16, //20
  36, //40
  60, //60
  87, //80
  114, //100
  137, //120
  161, ////140 
  185, //160
  207, //180
  221, //200
  224, //220
  };
float calculateSpeedoGaugeValue(float speedInKMH) {

  int lastSmallerIndex = kmhArraySize - 1;

  for (int i = 0; i < kmhArraySize - 2; i++) {
    if (speedInKMH >= kmhArray[i] && speedInKMH < kmhArray[i + 1]) {
      lastSmallerIndex = i;
      break;
    }
  }

  int smaller = kmhArray[lastSmallerIndex];
  int bigger = kmhArray[lastSmallerIndex + 1];
  float diffrence = bigger - smaller;
  float interpolationWave = (speedInKMH - smaller) / diffrence;

  return interpolateF(lastSmallerIndex, lastSmallerIndex+1, interpolationWave) * 20.0f;
}

float interpolateF(int a, int b, float wave) {
  return a + (b - a) * wave;
}

float remainderFloat(float x) {
  return x - (int)x;
}





void loop() {
  calculateValues();
}
