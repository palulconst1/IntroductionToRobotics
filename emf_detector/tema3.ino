const int pinA = 2;
const int pinB = 3;
const int pinC = 4;
const int pinD = 5;
const int pinE = 6;
const int pinF = 7;
const int pinG = 8;
const int pinDP = 9;

float maximReading = 50;
float minimReading = 27;

int const nrSamples = 5000;
const int emfPin = A5;
float emfVal = 0.0;
int readings[nrSamples];
unsigned long sumOfReadings; 

int digit = 0;
bool dpState = HIGH;

const int noOfDigits = 10;
const int segSize = 8;

const int buzzerPin = 11;
int buzzerTone = 0;
int buzzerToneMin = 300;
int buzzerToneMax = 500;
int buzzerState = LOW;

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

bool digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void displayNumber(byte digit, bool decimalPoint) {
  for(int i = 0; i<segSize - 1; i++){
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  digitalWrite(pinDP, decimalPoint);
}

void setup() {
  for(int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }

  pinMode(emfPin, INPUT);
  
  Serial.begin(9600);
}

void loop() {

for(int i = 0; i < nrSamples; i++){
   readings[i] = analogRead(emfPin);
   sumOfReadings += readings[i];
   }
     
  emfVal = sumOfReadings / nrSamples; 
  
  Serial.print("emfVal: ");
  Serial.println(emfVal);   

  emfVal = constrain(emfVal, minimReading, maximReading);                                              
  digit = map(emfVal, minimReading, maximReading, 0, 9);
  
  sumOfReadings = 0; 

  buzzerTone = map(digit, 0, 9, buzzerToneMin, buzzerToneMax);
  Serial.print("buzzerTone: ");
  Serial.println(buzzerTone); 
  displayNumber(digit, dpState);
  tone(buzzerPin, buzzerTone);
  
}
