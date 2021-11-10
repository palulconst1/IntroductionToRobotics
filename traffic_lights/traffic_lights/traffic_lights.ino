
const int buttonPin = 2;
int buttonState = 1;

const int buzzerPin = 3;
int buzzerTone = 300;
int buzzerState = LOW;

const int redCarPin = 9;
int redCarState = LOW;

const int yellowCarPin = 10;
int yellowCarState = LOW;

const int greenCarPin = 11;
int greenCarState = HIGH;

const int greenPeoplePin = 5;
int greenPeopleState = LOW;

const int redPeoplePin = 6;
int redPeopleState = HIGH;

bool reading = HIGH;
bool prevReading = HIGH;

int lastBounce = 0;
int bounceDelay = 50;

long startChangeStateTimer = 0;
long blinkTimer = 0;
long buzzerTimer = 0;

int changeStateOneDelay = 10000;
int changeStateTwoDelay = 3000;
int changeStateThreeDelay = 10000;
int changeStateFourDelay = 5000;
int blinkDelay = 200;
int buzzerDelay = 400;

bool stateOne = true;
bool stateTwo = false;
bool stateThree = false;
bool stateFour = false;
bool stateFive = false;

int turnedOnLedValue = 255;

void setup() {
    pinMode(buttonPin, INPUT_PULLUP);
    
    pinMode(greenCarPin, OUTPUT);
    pinMode(yellowCarPin, OUTPUT);
    pinMode(redCarPin, OUTPUT);

    pinMode(greenPeoplePin, OUTPUT);
    pinMode(redPeoplePin, OUTPUT);
    
    Serial.begin(9600);

}

void loop() {
// stateOne = start, cars: green | people: red
  if(stateOne) {
    analogWrite(greenCarPin, turnedOnLedValue);
    greenCarState = HIGH;
    analogWrite(redPeoplePin, turnedOnLedValue);
    redPeopleState = HIGH;
    
    reading = digitalRead(buttonPin);

    if (reading != prevReading) {
    lastBounce = millis();
    }
  
    if(millis() - lastBounce > bounceDelay) {
      if(reading != buttonState) {
        buttonState = reading;
        if (buttonState == LOW){
          stateOne = false;
          stateTwo = true;
          startChangeStateTimer = millis();
        }
      }
    }
    prevReading = reading;
  }

// stateTwo = button has been pressed and 10s passed, cars: yellow | people: red
  if(millis() - startChangeStateTimer > changeStateOneDelay && stateTwo) {
    analogWrite(greenCarPin, 0);
    greenCarState = LOW;
    analogWrite(yellowCarPin, turnedOnLedValue);
    yellowCarState = HIGH;

    stateTwo = false;
    stateThree = true;
    startChangeStateTimer = millis();
  }
// stateThree = yellow timer for cars finished, cars: red | people: green
    if(millis() - startChangeStateTimer > changeStateTwoDelay && stateThree) {
      analogWrite(yellowCarPin, 0);
      yellowCarState = LOW;
      analogWrite(redCarPin, turnedOnLedValue);
      redCarState = HIGH;
  
      analogWrite(redPeoplePin, 0);
      redPeopleState = LOW;
      analogWrite(greenPeoplePin, turnedOnLedValue);
      greenPeopleState = HIGH;
  
      stateThree = false;
      stateFour = true;
      startChangeStateTimer = millis();
      buzzerTimer = millis();
    }
// stateFour = start the buzzer, cars: red | people: green
    if(millis() - startChangeStateTimer < changeStateThreeDelay && stateFour) {
      Serial.println("bzz");
        if(millis() - buzzerTimer > buzzerDelay) {
          buzzerState = !buzzerState;
          buzzerTimer = millis();        
        }
        if(buzzerState == HIGH) {
          tone(buzzerPin, buzzerTone, 10);
        }
    }

    if(millis() - startChangeStateTimer > changeStateThreeDelay && stateFour) {
      stateFour = false;
      stateFive = true;
      blinkTimer = millis();
      startChangeStateTimer = millis();
  }
// stateFive = start the blinking, cars: red | people: green
    if(millis() - startChangeStateTimer < changeStateFourDelay && stateFive) {
        if(millis() - blinkTimer > blinkDelay) {
          greenPeopleState = !greenPeopleState;
          blinkTimer = millis();        
        }

        if(greenPeopleState == LOW) {
          analogWrite(greenPeoplePin, 0);
        } else {
          tone(buzzerPin, buzzerTone, 10);
          analogWrite(greenPeoplePin, turnedOnLedValue);
        }
   }
// stateFive = end of flow, go back to stage One, cars: green | people: red
    if(millis() - startChangeStateTimer > changeStateFourDelay && stateFive) {
      analogWrite(greenPeoplePin, 0);
      greenPeopleState = LOW;
      analogWrite(redCarPin, 0);
      redCarState = LOW;

      stateFive = false;
      stateOne = true;
      
      startChangeStateTimer = millis();
  }
  Serial.println(stateOne);
}
