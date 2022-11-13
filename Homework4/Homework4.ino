const int dataPin = 12;   //DS
const int latchPin = 11;  // STCP
const int clockPin = 10;  //SHCP

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int displayCount = 4;

int displayDigits[displayCount] = {
  segD1, segD2, segD3, segD4
};

int digitArray[16] = {
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
  B11101110,  // A
  B00111110,  // b
  B10011100,  // C
  B01111010,  // d
  B10011110,  // E
  B10001110   // F
};

int digits[displayCount] = {
  0, 0, 0, 0 
};

const int firstBlinkInterval = 400;
const int secondBlinkInterval = 800;

const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;

bool swState = LOW;
bool lastSwState = LOW;
byte switchState = HIGH;

int xValue = 0;
int yValue = 0;

bool joyMovedX = false;
bool joyMovedY = false;

int minThreshold = 400;
int maxThreshold = 600;

bool dpState = false;
bool selected = false;
int displayNo = 0;
unsigned long dpBlink = 0;

unsigned long lastDebounceTime = 0;
unsigned int debounceDelay = 50;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
const int longPressDuration = 1000;
const int shortPressDuration = 50;

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  moveY();
  swState = digitalRead(pinSW);
  buttonChangeState();
  writeNumber(displayNo, selected);
}

// function that moves the current digit according to the 
// movement of the joystick on the Y axis
void moveY() {
  if (yValue < minThreshold && !joyMovedY && !selected) {
    if (displayNo < 3) {
      displayNo++;
    }
    joyMovedY = true;
  }
  if (yValue > maxThreshold && !joyMovedY && !selected) {
    if (displayNo > 0) {
      displayNo--;
    }
    joyMovedY = true;
  }
  if (yValue >= minThreshold && yValue <= maxThreshold && !selected) {
    joyMovedY = false;
  }
}

// function that resets the display
void resetLeds() {
  digits[0] = 0;
  digits[1] = 0;
  digits[2] = 0;
  digits[3] = 0;
  displayNo = 0;
}

// function that changes state based on the button press
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
      if (pressDuration > longPressDuration && selected == 0) {
        Serial.println("long press");
        resetLeds();
      }
      else if (pressDuration > shortPressDuration) {
        Serial.println("short press");
        selected = !selected;
      }
    }
  }
  if (selected) {
    if (xValue > maxThreshold && !joyMovedX) {
      if (digits[displayNo] < 15) {
        digits[displayNo]++;
      }
      joyMovedX = true;
    }
    if (xValue < minThreshold && !joyMovedX) {
      if (digits[displayNo] > 0) {
        digits[displayNo]--;
      }
      joyMovedX = true;
    }
    if (xValue >= minThreshold && xValue <= maxThreshold) {
      joyMovedX = false;
    }
  }
  lastSwState = swState;
}

// function for writing the digits on the display
void writeNumber(int displayNo, int selected) {
  for (int i = 0; i < displayCount; i++) {
    int digitToWrite = 0;
    if (i == displayNo) { 
      if (selected) { // if the digit display is selected, the dp must be on
        digitToWrite = digitArray[digits[i]] + 1; // incrementing the value with 1, corresponding with the dp led
      }
      else { // if the digit display is not selected, the dp must blink
        digitToWrite = digitArray[digits[i]];
        if (millis() - dpBlink > firstBlinkInterval) {
          digitToWrite = digitArray[digits[i]] + 1;
        }
        if (millis() - dpBlink > secondBlinkInterval) {
          digitToWrite = digitArray[digits[i]];
          dpBlink = millis();
        }
      }
    }
    else {
      digitToWrite = digitArray[digits[i]];
    }
    writeReg(digitToWrite);
    showDigit(i);
    delay(5); // waiting for the electrical charge to run out
  }
}

// function used for displays
void showDigit(int displayNumber) {
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[displayNumber], LOW);
}

// function that writes the registers
void writeReg(int digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}