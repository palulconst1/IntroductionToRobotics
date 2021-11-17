#include <EEPROM.h>

const int dataPin = 12; // DS
const int latchPin = 11; // STPC
const int clockPin = 10; // SHCP

const int xPin = A0; 
const int yPin = A1;
const int swPin = 2;

const int joyMoveMin = 300;
const int joyMoveMax = 700;
bool joyMoved = LOW;

bool joyLeft = LOW;
bool joyRight = LOW;
bool joyUP = LOW;
bool joyDown = LOW;

int xValue = 0;
int yValue = 0;
int swState = HIGH;

int currentDigit = 0;
volatile bool digitSelected = LOW;
int shownDigits[] = {
  0, 0, 0, 0
};

int shownNumber = 1;

bool reading = HIGH;
bool prevReading = HIGH;

long lastBounce = 0;
const int bounceDelay = 50;

long blinkTimer = 0;
int blinkDelay = 200;

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int segmentDigits[] = {
  segD1, segD2, segD3, segD4
};

const int segmentsCount = 4;

int digitArray[16] = {
//A B C D E F G DP 
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // B
  B10011100, // C
  B01111010, // D
  B10011110, // E
  B00000001  // DP
};

bool inputROM = LOW;
int writeDelay = 1000;
long lastWrite = 0;

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  for(int i = 0; i < segmentsCount; i++) {
    pinMode(segmentDigits[i], OUTPUT);
    digitalWrite(segmentDigits[i], LOW);

    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);
    pinMode(swPin, INPUT_PULLUP); 
    attachInterrupt(digitalPinToInterrupt(swPin), buttonPressed, CHANGE);
  }
  Serial.begin(9600);
}

void loop() {
  if(!inputROM){
    readNumber();
    inputROM = HIGH;
  }

  Serial.println(digitSelected);
  
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
    
//  reading = digitalRead(swPin);
//
//  if (reading != prevReading) {
//  lastBounce = millis();
//  }
//
//  if(millis() - lastBounce > bounceDelay) {
//    if(reading != swState) {
//      swState = reading;
//      if(reading == LOW) {
//        digitSelected = !digitSelected;
//  }
//    }
//  }
//  prevReading = reading;

  if(xValue > joyMoveMin && xValue < joyMoveMax && yValue > joyMoveMin && yValue < joyMoveMax) {
    joyMoved = LOW;
    //Serial.println("neutral");
  }
  if(joyMoved == LOW) {
    if(xValue < joyMoveMin && !digitSelected) {
      joyLeft = HIGH;
      joyMoved = HIGH;
      if(currentDigit > 0){
        currentDigit--;
      }
    }
    if(xValue > joyMoveMax && !digitSelected) {
      joyRight = HIGH;
      joyMoved = HIGH;
      if(currentDigit < 3){
        currentDigit++;
      }
    }
    if(yValue < joyMoveMin && digitSelected) {
      joyDown = HIGH;
      joyMoved = HIGH;
      if(shownDigits[currentDigit] > 0){
        shownDigits[currentDigit]--;
      }
    }
    if(yValue > joyMoveMax && digitSelected) {
      joyUP = HIGH;
      joyMoved = HIGH;
      if(shownDigits[currentDigit] < 9){
        shownDigits[currentDigit]++;
      }
    }
  }

  if(digitSelected)
  {
    showDigit(3 - currentDigit);
    writeReg(digitArray[15]);
  }else {
      if(millis() - blinkTimer > blinkDelay) {
        showDigit(3 - currentDigit);
        writeReg(digitArray[15]);
        blinkTimer = millis();
      }
  }

  for(int i = 0; i < segmentsCount; i++) {
    shownNumber *= 10;
    shownNumber += shownDigits[i];
  }
  writeNumber(shownNumber);
  //Serial.println(shownNumber);
  shownNumber = 1;

  if(millis() - lastWrite > writeDelay){
    saveNumber();
    lastWrite = millis();
  }
}

void buttonPressed() {
  Serial.println(112);
  if(millis() - lastBounce > 500) {
    digitSelected = !digitSelected;
  }
  lastBounce = millis();
}

void writeNumber(int number) {
  int currentNumber = number;
  int lastDigit;
  int displayDigit = 0;
  while (currentNumber > 0) {
    lastDigit = currentNumber % 10;
    showDigit(displayDigit);
    writeReg(digitArray[lastDigit]);
    displayDigit ++;
    currentNumber /= 10;
    delay(5);
  }
}

void showDigit(int digitNumber) {
  for(int i = 0; i< segmentsCount; i++) {
    digitalWrite(segmentDigits[i], HIGH);
  }
  digitalWrite(segmentDigits[digitNumber], LOW);
}

void writeReg(int digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}

void saveNumber(){
  for(int i = 0; i < segmentsCount; i++){
    EEPROM.write(i, shownDigits[i]);
  }
}

void readNumber(){
  for(int i = 0; i < segmentsCount; i++){
    shownDigits[i] = EEPROM.read(i);
  }
}
