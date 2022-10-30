const int buzzerPin = 3;
const int buttonPin = 2;
// pins for the car leds
const int carRedLedPin = 11;
const int carYellowLedPin = 10;
const int carGreenLedPin = 9;
// pins for the person leds
const int personRedLedPin = 6;
const int personGreenLedPin = 5; 
byte ledState = HIGH; // blinking person green led state
byte buttonState = LOW;
byte buttonReading = LOW;
byte buttonLastReading = HIGH;
unsigned long lastDebounceTime = 0;
unsigned int debounceDelay = 50;
int state = 1; // to see what state the program is in 
unsigned long startingTime = 0; // the time since the state the program is in started 
unsigned long beepTime = 0; // the time since the last beep was played
const int beepFirstFreq = 1000; 
const int beepSecondFreq = 300;
const int buzzerFirstTone = 1100;
const int buzzerSecondTone = 1400;
const int buzzerFirstLength = 400;
const int buzzerSecondLength = 200;
const int state1Time = 8000; 
const int state2Time = 3000;
const int state3Time = 8000;
const int state4Time = 4000;

void setup() {
  pinMode(carRedLedPin, OUTPUT);
  pinMode(carYellowLedPin, OUTPUT);
  pinMode(carGreenLedPin, OUTPUT);
  pinMode(personRedLedPin, OUTPUT);
  pinMode(personGreenLedPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(carGreenLedPin, HIGH);
  digitalWrite(personRedLedPin, HIGH);
  Serial.begin(9600);
}

void state2() {
  if ((millis() - startingTime) >= state2Time) {
    Serial.println("state 1 to state 2");
    state = 3;
    startingTime = millis();
    beepTime = millis();
    digitalWrite(carYellowLedPin, LOW);
    digitalWrite(carRedLedPin, HIGH);
    digitalWrite(personRedLedPin, LOW);
    digitalWrite(personGreenLedPin, HIGH);
  }
}

void state3() {
  if ((millis() - beepTime) >= beepFirstFreq) {
    tone(buzzerPin, buzzerFirstTone, buzzerFirstLength);
    beepTime = millis();
  } 
  if ((millis() - startingTime) >= state3Time) {
    state = 4;
    Serial.println("state 2 to state 3");
    startingTime = millis();
    beepTime = millis();
  }
}

void state4() {
  if (millis() - beepTime >= beepSecondFreq) {
      tone(buzzerPin, buzzerSecondTone, buzzerSecondLength);
      digitalWrite(personGreenLedPin, ledState);
      ledState = !ledState;
      beepTime = millis();
    }
    if (millis() - startingTime >= state4Time) {
      Serial.println("state 3 to state 0");
      state = 1;
      digitalWrite(carRedLedPin, LOW);
      digitalWrite(carGreenLedPin, HIGH);
      digitalWrite(personRedLedPin, HIGH);
      digitalWrite(personGreenLedPin, LOW);
      ledState = HIGH;
    }
}

void loop () {
  buttonReading = digitalRead(buttonPin);
  if (buttonReading != buttonLastReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonReading != buttonState) {
      buttonState = buttonReading;
      if (buttonState == LOW) {
        if (state == 1) {
          Serial.println("state 0 to state 1");
          //delay(8000);
          startingTime = millis();
          while (millis() - startingTime < state1Time) {
          }
          startingTime = millis();
          state = 2;
          digitalWrite(carYellowLedPin, HIGH);
          digitalWrite(carGreenLedPin, LOW);
        }
      }
    }
  }
  if (state == 2)
    state2();

  if (state == 3)
    state3();

  if (state == 4)
    state4();

  buttonLastReading = buttonReading;
}