const int pinSW = 2;
const int pinX = A0; 
const int pinY = A1;
// declare all the segments pins and states
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;
const int segSize = 8;
byte ledAState = LOW;
byte ledBState = LOW;
byte ledCState = LOW;
byte ledDState = LOW;
byte ledEState = LOW;
byte ledFState = LOW;
byte ledGState = LOW;
byte ledDPState = LOW;

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

// declare variables for blinking state
int currentBlinkPin = 11;
byte blinkLedState = LOW;
byte doNotBlink = LOW;

// declare variables for joystick potentiometers
int xValue = 0;
int yValue = 0;
int minThreshold = 400;
int maxThreshold = 600;
byte joyBackToMiddleYstate2 = LOW;
byte joyBackToMiddleYstate1 = LOW;
byte joyBackToMiddleX = LOW;

// declare variables for joystick switch
byte swState = LOW;
byte lastSwState = HIGH;
byte switchState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned int debounceDelay = 50;
unsigned int blinkInterval = 400;
unsigned long lastBlinkTime = 0;
unsigned long pressedTime;
unsigned long releasedTime;
unsigned long noOfPresses = 0;
const int longPressDuration = 1000;
const int shortPressDuration = 50;

int state = 1;

void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
}

// function that displays the state of all the segments
void displayLeds() {
  if (currentBlinkPin != pinA)
    digitalWrite(pinA, ledAState);
  if (currentBlinkPin != pinB)
    digitalWrite(pinB, ledBState);
  if (currentBlinkPin != pinC)
    digitalWrite(pinC, ledCState);
  if (currentBlinkPin != pinD)
    digitalWrite(pinD, ledDState);
  if (currentBlinkPin != pinE)
    digitalWrite(pinE, ledEState);
  if (currentBlinkPin != pinF)
    digitalWrite(pinF, ledFState);
  if (currentBlinkPin != pinG)
    digitalWrite(pinG, ledGState);
  if (currentBlinkPin != pinDP)
    digitalWrite(pinDP, ledDPState);
  digitalWrite(currentBlinkPin, blinkLedState);
}

// function that resets the state of all the segments
// and moves the blinking to the decimal point
void resetLeds() {
  ledAState = LOW;
  ledBState = LOW;
  ledCState = LOW;
  ledDState = LOW;
  ledEState = LOW;
  ledFState = LOW;
  ledGState = LOW;
  ledDPState = LOW;
  currentBlinkPin = pinDP;
}

// function that moves the blinking segment according
// to the movement of the joystick
void moveBlinkingState1() {
  if (yValue < minThreshold && joyBackToMiddleYstate1 == LOW && state == 1) {
    if (currentBlinkPin == pinA) {
      currentBlinkPin = pinF;
      Serial.println("left from A to F");
    }
    else if (currentBlinkPin == pinB) {
      currentBlinkPin = pinF;
      Serial.println("left from B to F");
    }
    else if (currentBlinkPin == pinC) {
      currentBlinkPin = pinE;
      Serial.println("left from C to E");
    }
    else if (currentBlinkPin == pinDP) {
      currentBlinkPin = pinC;
      Serial.println("left from DP to C");
    }
    else if (currentBlinkPin == pinD) {
      currentBlinkPin = pinE;
      Serial.println("left from D to E");
    }
    joyBackToMiddleYstate1 = HIGH;
  }
  else if (yValue > maxThreshold && joyBackToMiddleYstate1 == LOW && state == 1) {
    if (currentBlinkPin == pinA || currentBlinkPin == pinF) {
      currentBlinkPin = pinB;
      Serial.println("right from A to B");
    }
    else if (currentBlinkPin == pinF) {
      currentBlinkPin = pinB;
      Serial.println("right from F to B");
    }
    else if (currentBlinkPin == pinC) {
      currentBlinkPin = pinDP;
      Serial.println("right from C to DP");
    }
    else if (currentBlinkPin == pinE) {
      currentBlinkPin = pinC;
      Serial.println("right from E to C");
    }
    else if (currentBlinkPin == pinD) {
      currentBlinkPin = pinC;
      Serial.println("right from D to C");
    }
    joyBackToMiddleYstate1 = HIGH; 
  }
  else if (joyBackToMiddleYstate1 == HIGH && yValue < maxThreshold && yValue > minThreshold && state == 1) {
    joyBackToMiddleYstate1 = LOW;
  }

  if (xValue < minThreshold && joyBackToMiddleX == LOW && state == 1) {
    if (currentBlinkPin == pinG) {
      currentBlinkPin = pinD;
      Serial.println("down from G to D");
    }
    else if (currentBlinkPin == pinA) {
      currentBlinkPin = pinG;
      Serial.println("down from A to G");
    }
    else if (currentBlinkPin == pinB) {
      currentBlinkPin = pinG;
      Serial.println("down from B to G");
    }
    else if (currentBlinkPin == pinF) {
      currentBlinkPin = pinG;
      Serial.println("down from F to G");
    }
    else if (currentBlinkPin == pinC) {
      currentBlinkPin = pinD;
      Serial.println("down from C to D");
    }
    else if (currentBlinkPin == pinE) {
      currentBlinkPin = pinD;
      Serial.println("down from E to D");
    }
    joyBackToMiddleX = HIGH;
  }
  else if (xValue > maxThreshold && joyBackToMiddleX == LOW && state == 1) {
    if (currentBlinkPin == pinG) {
      currentBlinkPin = pinA;
      Serial.println("up from G to A");
    }
    else if (currentBlinkPin == pinD) {
      currentBlinkPin = pinG;
      Serial.println("up from D to G");
    }
    else if (currentBlinkPin == pinE) {
      currentBlinkPin = pinG;
      Serial.println("up from E to G");
    }
    else if (currentBlinkPin == pinC) {
      currentBlinkPin = pinG;
      Serial.println("up from C to G");
    }
    else if (currentBlinkPin == pinB) {
      currentBlinkPin = pinA;
      Serial.println("up from B to A");
    }
    else if (currentBlinkPin == pinF) {
      currentBlinkPin = pinA;
      Serial.println("up from F to A");
    }
    joyBackToMiddleX = HIGH;
  }
  else if (joyBackToMiddleX == HIGH && xValue < maxThreshold && xValue > minThreshold && state == 1) {
    joyBackToMiddleX = LOW;
  }
}

// function that turns on or off the segment
// by using the joystick movement on the y axis (left - off, right - on)
void turnOnOffState2() {
  if (yValue < minThreshold && joyBackToMiddleYstate2 == LOW && state == 2) {
    blinkLedState = LOW;
    if (currentBlinkPin == pinA) {
      ledAState = LOW;
      Serial.println("off A");
    }
    else if (currentBlinkPin == pinB) {
      ledBState = LOW;
      Serial.println("off B");
    }
    else if (currentBlinkPin == pinC) {
      ledCState = LOW;
      Serial.println("off C");
    }
    else if (currentBlinkPin == pinD) {
      ledDState = LOW;
      Serial.println("off D");
    }
    else if (currentBlinkPin == pinE) {
      ledEState = LOW;
      Serial.println("off E");
    }
    else if (currentBlinkPin == pinF) {
      ledFState = LOW;
      Serial.println("off F");
    }
    else if (currentBlinkPin == pinG) {
      ledGState = LOW;
      Serial.println("off G");
    }
    else if (currentBlinkPin == pinDP) {
      ledDPState = LOW;
      Serial.println("off DP");
    }
    joyBackToMiddleYstate2 = HIGH;
  }
  else if (yValue > maxThreshold && joyBackToMiddleYstate2 == LOW && state == 2) {
    blinkLedState = HIGH;
    if (currentBlinkPin == pinA) {
      ledAState = HIGH;
      Serial.println("on A");
    }
    else if (currentBlinkPin == pinB) {
      ledBState = HIGH;
      Serial.println("on B");
    }
    else if (currentBlinkPin == pinC) {
      ledCState = HIGH;
      Serial.println("on C");
    }
    else if (currentBlinkPin == pinD) {
      ledDState = HIGH;
      Serial.println("on D");
    }
    else if (currentBlinkPin == pinE) {
      ledEState = HIGH;
      Serial.println("on E");
    }
    else if (currentBlinkPin == pinF) {
      ledFState = HIGH;
      Serial.println("on F");
    }
    else if (currentBlinkPin == pinG) {
      ledGState = HIGH;
      Serial.println("on G");
    }
    else if (currentBlinkPin == pinDP) {
      ledDPState = HIGH;
      Serial.println("on DP");
    }
    joyBackToMiddleYstate2 = HIGH; 
  }
  else if (joyBackToMiddleYstate2 == HIGH && yValue < maxThreshold && yValue > minThreshold && state == 2) {
    joyBackToMiddleYstate2 = LOW;
  }
}

// function that changes the state based on
// how the button is pressed 
// (long press - reset, short press - enter state 1)
void buttonChangeState() {
  if (swState != lastSwState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (swState != switchState) {
      switchState = swState;
      if (switchState == LOW) {
        pressedTime = millis();
      }
      else {
        releasedTime = millis();
      }
      long pressDuration = releasedTime - pressedTime;
      if (pressDuration > longPressDuration and state == 1) {
        Serial.println("long press");
        resetLeds();
      }
      else if (pressDuration > shortPressDuration) {
        Serial.println("short press");
        noOfPresses ++;
        if (noOfPresses % 2 != 0) {
          doNotBlink = HIGH;
          state = 2;
        }
        else {
          doNotBlink = LOW;
          state = 1;
        }
      }
    }
  }
  
  lastSwState = swState;
}

void blink() {
  if (millis() - lastBlinkTime > blinkInterval && doNotBlink == LOW) {
    blinkLedState = !blinkLedState;
    lastBlinkTime = millis();
  }
}

void loop() {
  displayLeds();

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  blink();

  moveBlinkingState1();

  turnOnOffState2();

  swState = digitalRead(pinSW);

  buttonChangeState(); 
}
