const int redInputPin = A2;
const int greenInputPin = A1;
const int blueInputPin = A0;
const int redLedPin = 11;
const int greenLedPin = 10;
const int blueLedPin = 9;
int redInputValue;
int greenInputValue;
int blueInputValue;
int redLedValue;
int greenLedValue;
int blueLedValue;
const int maxInitialValue = 1023;
const int maxValue = 255;
const int minInitialValue = 0;
const int minValue = 0;

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redLedPin, redValue); 
  analogWrite(greenLedPin, greenValue);
  analogWrite(blueLedPin, blueValue);
}

void setup() {
  pinMode(redInputPin, INPUT);
  pinMode(greenInputPin, INPUT);
  pinMode(blueInputPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
}

void loop() {
  redInputValue = analogRead(redInputPin);
  greenInputValue = analogRead(greenInputPin);
  blueInputValue = analogRead(blueInputPin);
  redLedValue = map(redInputValue, minInitialValue, maxInitialValue, minValue, maxValue);
  greenLedValue = map(greenInputValue, minInitialValue, maxInitialValue, minValue, maxValue);
  blueLedValue = map(blueInputValue, minInitialValue, maxInitialValue, minValue, maxValue);
  setColor(redLedValue, greenLedValue, blueLedValue);
}