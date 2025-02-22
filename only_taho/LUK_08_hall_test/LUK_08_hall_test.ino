int PRZEBIEG = 5;
int KIERUNEK_0 = 4;
int KIERUNEK_1 = 9;
int ENABLE_TAHO = 11;
int ENABLE_SPEEDO = 10;
int SPEEDO_SIGNAL_IN = 2;  //hall kmh
int TAHO_SIGNAL_IN = 3;    //hall rpm
//13 wbudowana dioda


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
  test();
  test();
}

void test() {
  unsigned long loopTime = millis();

  while (loopTime + 1000 > millis()) {
    analogWrite(ENABLE_SPEEDO, 255);
    analogWrite(ENABLE_TAHO, 255);
  }

  while (loopTime + 2000 > millis()) {
    analogWrite(ENABLE_SPEEDO, 32);
    analogWrite(ENABLE_TAHO, 32);
  }

  while (loopTime + 3000 > millis()) {
    analogWrite(ENABLE_SPEEDO, 64);
    analogWrite(ENABLE_TAHO, 64);
  }

  while (loopTime + 4000 > millis()) {
    analogWrite(ENABLE_SPEEDO, 96);
    analogWrite(ENABLE_TAHO, 96);
  }

  while (loopTime + 5000 > millis()) {
    analogWrite(ENABLE_SPEEDO, 128);
    analogWrite(ENABLE_TAHO, 128);
  }

  while (loopTime + 6000 > millis()) {
    analogWrite(ENABLE_SPEEDO, 160);
    analogWrite(ENABLE_TAHO, 160);
  }

  while (loopTime + 7000 > millis()) {
    analogWrite(ENABLE_SPEEDO, 192);
    analogWrite(ENABLE_TAHO, 192);
  }

  while (loopTime + 8000 > millis()) {
    analogWrite(ENABLE_SPEEDO, 0);
    analogWrite(ENABLE_TAHO, 0);
  }
  analogWrite(ENABLE_SPEEDO, 0);
  analogWrite(ENABLE_TAHO, 0);
}

volatile int speedoCounter = 0;
volatile int tahoCounter = 0;

void przerwanieSpeedo() {
  if (speedoCounter == 0)
    speedoCounter = 1;
  else
    speedoCounter = 0;
}

void przerwanieTaho() {
  if (tahoCounter == 0)
    tahoCounter = 1;
  else
    tahoCounter = 0;
}

void loop() {
  if (speedoCounter == 0)
    analogWrite(ENABLE_SPEEDO, 0);
  else
    analogWrite(ENABLE_SPEEDO, 255);

  if (tahoCounter == 0)
    analogWrite(ENABLE_TAHO, 0);
  else
    analogWrite(ENABLE_TAHO, 255);
}
