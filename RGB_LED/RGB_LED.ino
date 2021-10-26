const int ledPinRed = 11;
const int ledPinGreen = 10;
const int ledPinBlue = 9;

const int potPinRed = A1;
const int potPinGreen = A2;
const int potPinBlue = A3;

int potValueRed = 0;
int potValueGreen = 0;
int potValueBlue = 0;

float voltageRed = 0.0;
float voltageGreen = 0.0;
float voltageBlue = 0.0;

int brightnessRed = 0;
int brightnessGreen = 0;
int brightnessBlue = 0;

float maxAnalogValue = 1023.0;
float maxVoltageVaue = 5.0;

void setup() {
  pinMode(ledPinRed, OUTPUT);
  pinMode(potPinRed, INPUT);
  
  pinMode(ledPinGreen, OUTPUT);
  pinMode(potPinGreen, INPUT);
  
  pinMode(ledPinBlue, OUTPUT);
  pinMode(potPinBlue, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  potValueRed = analogRead(potPinRed);
  potValueGreen = analogRead(potPinGreen);
  potValueBlue = analogRead(potPinBlue);
  
  brightnessRed = map(potValueRed, 0, 1023, 0, 255);
  brightnessGreen = map(potValueGreen, 0, 1023, 0, 255);
  brightnessBlue = map(potValueBlue, 0, 1023, 0, 255);
      
  analogWrite(ledPinRed, brightnessRed);
  analogWrite(ledPinGreen, brightnessGreen);
  analogWrite(ledPinBlue, brightnessBlue);

  voltageRed = potValueRed * maxVoltageVaue / maxAnalogValue;
  voltageGreen = potValueGreen * maxVoltageVaue / maxAnalogValue;
  voltageBlue = potValueBlue * maxVoltageVaue / maxAnalogValue;

//  Serial.print("Red voltage: ");
//  Serial.print(voltageRed);
//  Serial.println();
//
//  Serial.print("Green voltage: ");
//  Serial.print(voltageGreen);
//  Serial.println();
//
//  Serial.print("Blue voltage: ");
//  Serial.print(voltageBlue);
//  Serial.println();
}
