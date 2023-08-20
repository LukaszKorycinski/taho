int PRZEBIEG = 3;

int KIERUNEK_0 = 4;
int KIERUNEK_1 = 9;
int ENABLE_TAHO = 11;
int ENABLE_SPEEDO = 10;

int TAHO_SIGNAL_IN = 6;//masa z cewki

void setup() {
  pinMode(PRZEBIEG, OUTPUT);
  pinMode(ENABLE_TAHO, OUTPUT);
  pinMode(KIERUNEK_0, OUTPUT);
  pinMode(KIERUNEK_1, OUTPUT);
  pinMode(ENABLE_SPEEDO, OUTPUT);
  pinMode(TAHO_SIGNAL_IN, INPUT_PULLUP);

  digitalWrite(PRZEBIEG, LOW);
  digitalWrite(ENABLE_TAHO, LOW);
  digitalWrite(KIERUNEK_0, LOW);
  digitalWrite(KIERUNEK_1, HIGH);
  digitalWrite(ENABLE_SPEEDO, LOW);

  initGauges();
  test();
  initGauges();

  while(true){
    normalLoop();
    }
}

unsigned long rpmTime = 0;
unsigned long fixedRPM = 0;

bool tahoMassValue = false;
bool tahoMassOldValue = false;

void initGauges() {
  rpmTime = micros();
}

unsigned long fromTimeToRPM(unsigned long rotationMillis) {
  return 60000000 / rotationMillis;
}

void callRPM() {
  tahoMassValue = digitalRead(TAHO_SIGNAL_IN);

  if ( tahoMassOldValue != tahoMassValue ) {
    tahoMassOldValue = tahoMassValue;
    if (tahoMassValue == false) {
      unsigned long rotationTime =  micros() - rpmTime;
      fixedRPM = fromTimeToRPM(rotationTime);
      rpmTime = micros();
    }
  }
}


int rpmArraySize = 16;
//                            0  10  20   30    40    50    60    70    80    90   100   110   120    130    140   150
unsigned long rpmArray[16] = {0, 10, 20, 800, 1900, 3000, 4000, 5200, 6500, 7500, 8500, 9200, 10000, 11200, 12300, 13500};


double toRpmGaugeValue(unsigned long rpmValue) {

  int lastSmallerIndex = rpmArraySize - 1;
  if (rpmValue == 0ul) {
    lastSmallerIndex = 0;
  }
  rpmValue = min(rpmValue, rpmArray[rpmArraySize - 1]);

  for (int i = 0; i < rpmArraySize - 1; i++) {
    if (rpmValue > rpmArray[i] && rpmValue <= rpmArray[i + 1]) {
      lastSmallerIndex = i;
    }
  }

  unsigned long biasStart = rpmValue - rpmArray[lastSmallerIndex];                  //9000 - 8500 = 500     // obroty - smaller
  unsigned long biasEnd = rpmArray[lastSmallerIndex + 1] - rpmArray[lastSmallerIndex]; //9200 - 8500 = 700   // bigger - smaller
  
  double bias = ((double)biasStart) / ((double)biasEnd); // 500/700=0.71

  double mullOne = 1.0 - bias;//0.29
  double mullTwo = bias;//0.71

  return (lastSmallerIndex * 10) * mullOne + ((lastSmallerIndex + 1) * 10) * mullTwo ; // 10*10*0.29 + 11*10*0.71 = 29 + 78 =  107   
}


void test() {
  unsigned long loopTime = millis();

  while (loopTime + 1000 > millis()) {
    analogWrite(ENABLE_TAHO, ((int)toRpmGaugeValue(16000)));
  }
  analogWrite(ENABLE_TAHO, ((int)toRpmGaugeValue(0)));
}


void normalLoop(){
  callRPM();

  analogWrite(ENABLE_TAHO, ((int)toRpmGaugeValue(fixedRPM)));
  }

void loop() {
//  normalLoop();
}
