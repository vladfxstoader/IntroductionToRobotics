#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "pitches.h"
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
const int buzzerPin = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER
byte matrixBrightness = 1;
const byte matrixHello[matrixSize] = {
  B10010001,
  B10010001,
  B10010101,
  B10010001,
  B11110101,
  B10010101,
  B10010100,
  B10010101
};
const byte matrixMenu[5][8] = {
  {
    0b00100000,
    0b00110000,
    0b00111000,
    0b00111100,
    0b00111100,
    0b00111000,
    0b00110000,
    0b00100000
  },
  {
    0b01111100,
    0b00111000,
    0b00010000,
    0b01111100,
    0b01010100,
    0b00111000,
    0b00010000,
    0b00111000
  },
  {
    0b00010000,
    0b00111000,
    0b01110000,
    0b11110000,
    0b01001000,
    0b00000100,
    0b00000010,
    0b00000001
  },
  {
    0b00000000,
    0b00011000,
    0b00100100,
    0b00000100,
    0b00001000,
    0b00001000,
    0b00000000,
    0b00001000
  },
  {
    0b00000000,
    0b00001000,
    0b00000000,
    0b00011000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00011100
  }
};

const byte matrixHappy[8] = {
  0b00100100,
  0b00100100,
  0b00100100,
  0b00100100,
  0b00000000,
  0b10000001,
  0b01000010,
  0b00111100
};

byte matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}  
};

const String menuOptions[5] = {
  "Start game",
  "Leaderboard",
  "Settings",
  "How to play",
  "About"
};
const String settingsOptions[5] = {
  "Player name",
  "LCD bright.",
  "Matrix bright.",
  "Difficulty",
  "Sounds"
};
String name = "AAA";
char nameLetters[3] = {
  'A', 'A', 'A'
};
int namePos = 0;
int menuCurrentItem = 0;
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;
bool swState = LOW;
bool lastSwState = LOW;
int state = 0;
byte switchState = HIGH;
int xValue = 0;
int subMenuOption = 0;
int yValue = 0;
bool joyBackToMiddleX = LOW;
bool joyBackToMiddleY = LOW;
int minThreshold = 400;
int maxThreshold = 600;
unsigned long lastDebounceTime = 0;
unsigned int debounceDelay = 50;

bool matrixChanged = true;
unsigned int score = 0;
byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;
byte lastFoodRow = 0;
byte lastFoodColumn = 0;
byte currentFoodRow = 0;
byte leaderboard = 0;
int leaderboardPos = 0;
byte settings = 0;
int settingsPos = 0;
byte currentFoodColumn = 0;
byte startGame = 0;
char * messageAbout = "                Made by Vlad Toader. Github user: @vladfxstoader                ";
char * messageHTP = "                Eat the blinking food to increase the score. When reaching 10, the game is over.                ";
const byte moveInterval = 100;
int LCDbrightness;
int difficulty = 1;
byte sounds;
unsigned long long lastBlink = 0;
byte blinkLetter = LOW;
const int blinkInterval = 250;
unsigned long long lastMoved = 0;
int highscores[5];
String highscoreNames[5] = {"", "", "", "", ""};
unsigned long lastLetterBlink = 0;

/*
in EEPROM, the memory will be put like this:
1 - LCD brightness level (0-5, multiplied by 51 and write it to pin)
2 - Matrix brightness
3 - Difficulty
4 - Sounds on/off
5 - First highscore
6 -> 9 - First highscore name
10 - Second highscore
11 -> 14 - Second highscore name
15 - Third highscore
16 -> 19 - Third highscore name
20 - Fourth highscore
21 -> 24 - Fourth highscore name
25 - Fifth highscore
26 -> 29 - Fifth highscore name
*/

// function that gets the settings values from EEPROM
void getSettingsFromEEPROM() {
  LCDbrightness = EEPROM.read(1);
  matrixBrightness = EEPROM.read(2);
  difficulty = EEPROM.read(3);
  sounds = EEPROM.read(4);
}

// function used for initialising the leaderboard with 0
void putInitialHStoEEPROM() {
  for (int i = 1; i <= 5; ++ i) {
    EEPROM.update(i * 5, 0);
    EEPROM.update(i * 5 + 1, 'N');
    EEPROM.update(i * 5 + 2, 'a');
    EEPROM.update(i * 5 + 3, 'N');
  }
}

// function that updates the highscores at the end of the game
void updateHStoEEPROM(int newScore, String newName) {
  if (newScore > highscores[0]) {
    EEPROM.update(25, EEPROM.read(20));
    EEPROM.update(26, EEPROM.read(21));
    EEPROM.update(27, EEPROM.read(22));
    EEPROM.update(28, EEPROM.read(23));

    EEPROM.update(20, EEPROM.read(15));
    EEPROM.update(21, EEPROM.read(16));
    EEPROM.update(22, EEPROM.read(17));
    EEPROM.update(23, EEPROM.read(18));

    EEPROM.update(15, EEPROM.read(10));
    EEPROM.update(16, EEPROM.read(11));
    EEPROM.update(17, EEPROM.read(12));
    EEPROM.update(18, EEPROM.read(13));

    EEPROM.update(10, EEPROM.read(5));
    EEPROM.update(11, EEPROM.read(6));
    EEPROM.update(12, EEPROM.read(7));
    EEPROM.update(13, EEPROM.read(8));
    
    EEPROM.update(5, newScore);
    EEPROM.update(6, newName[0]);
    EEPROM.update(7, newName[1]);
    EEPROM.update(8, newName[2]);
  }
  else if (newScore > highscores[1]) {
    EEPROM.update(25, EEPROM.read(20));
    EEPROM.update(26, EEPROM.read(21));
    EEPROM.update(27, EEPROM.read(22));
    EEPROM.update(28, EEPROM.read(23));

    EEPROM.update(20, EEPROM.read(15));
    EEPROM.update(21, EEPROM.read(16));
    EEPROM.update(22, EEPROM.read(17));
    EEPROM.update(23, EEPROM.read(18));

    EEPROM.update(15, EEPROM.read(10));
    EEPROM.update(16, EEPROM.read(11));
    EEPROM.update(17, EEPROM.read(12));
    EEPROM.update(18, EEPROM.read(13));

    EEPROM.update(10, newScore);
    EEPROM.update(11, newName[0]);
    EEPROM.update(12, newName[1]);
    EEPROM.update(13, newName[2]);
  }
  else if (newScore > highscores[2]) {
    EEPROM.update(25, EEPROM.read(20));
    EEPROM.update(26, EEPROM.read(21));
    EEPROM.update(27, EEPROM.read(22));
    EEPROM.update(28, EEPROM.read(23));

    EEPROM.update(20, EEPROM.read(15));
    EEPROM.update(21, EEPROM.read(16));
    EEPROM.update(22, EEPROM.read(17));
    EEPROM.update(23, EEPROM.read(18));

    EEPROM.update(15, newScore);
    EEPROM.update(16, newName[0]);
    EEPROM.update(17, newName[1]);
    EEPROM.update(18, newName[2]);
  }
  else if (newScore > highscores[3]) {
    EEPROM.update(25, EEPROM.read(20));
    EEPROM.update(26, EEPROM.read(21));
    EEPROM.update(27, EEPROM.read(22));
    EEPROM.update(28, EEPROM.read(23));

    EEPROM.update(20, newScore);
    EEPROM.update(21, newName[0]);
    EEPROM.update(22, newName[1]);
    EEPROM.update(23, newName[2]);
  }
  else if (newScore > highscores[4]) {
    EEPROM.update(25, newScore);
    EEPROM.update(26, newName[0]);
    EEPROM.update(27, newName[1]);
    EEPROM.update(28, newName[2]);
  }
}

// function that retrieves the highscores from EEPROM
void getHighscores() {
  for (int i = 1; i <= 5; ++ i) {
    highscores[i-1] = EEPROM.read(i * 5);
    int firstLetter = EEPROM.read(i * 5 + 1);
    int secondLetter = EEPROM.read(i * 5 + 2);
    int thirdLetter = EEPROM.read(i * 5 + 3);
    String name = "";
    name += char(firstLetter);
    name += char(secondLetter);
    name += char(thirdLetter);
    highscoreNames[i-1] = name;
    Serial.print(highscores[i-1]);
    Serial.print(" - ");
    Serial.println(highscoreNames[i-1]);
  }
}

// function that plays a song used for the welcome screen
void song() {
  int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  };
  int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
  };
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}

// function used to blink the current letter when the player
// chooses their name
void blinkLetterName() {
  if (state == 3 && settings == 1 && settingsPos == 0) {
    if (millis() - lastLetterBlink > 400) {
      lastLetterBlink = millis();
      blinkLetter = !blinkLetter;
    }
    lcd.setCursor(namePos+4, 1);
    if (blinkLetter == HIGH) {
      lcd.print(nameLetters[namePos]);
    }
    else {
        lcd.print(" ");
    }
  }
}

// function used for different uses of the button of the joystick
void buttonLogic() {
  if (swState != lastSwState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (swState != switchState) {
      switchState = swState;
      if (switchState == LOW) {
        if (state == 0 && startGame == 0) { // past the welcome screen
          state = 1;
          lcd.clear();
          lcd.print("<MENU>");          
          lcd.setCursor(0, 1);
          lcd.print(">");
          lcd.print(menuOptions[menuCurrentItem]);
          lc.clearDisplay(0);
          for (int row = 0; row < matrixSize; row++) {
            lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
          }
        }
        else if (subMenuOption == 1) { // when the game starts
          startGame = 1;
          lcd.clear();
          lcd.print("PLAYER: ");
          lcd.print(nameLetters[0]);
          lcd.print(nameLetters[1]);
          lcd.print(nameLetters[2]);
          lcd.setCursor(0, 1);
          lcd.print("SCORE: ");
          lcd.print(score);
          lc.clearDisplay(0);// clear screen
          matrix[xPos][yPos] = 1;
          generateFood();
        }
        else if (state == 3 && startGame == 0 && settings == 1 && settingsPos == 0) { // after the player saves their name
          state = 2;
          lcd.clear();
          lcd.print("<SETTINGS>");
          lcd.setCursor(0, 1);
          lcd.print(">");
          lcd.print(settingsOptions[settingsPos]);
          lc.clearDisplay(0);
            for (int row = 0; row < matrixSize; row++) {
              lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
            }
        }
      }
    }
  }
  lastSwState = swState;
}

// function used for different uses of the y axis of the joystick
void yAxisLogic() {
  if (yValue > maxThreshold && joyBackToMiddleY == LOW && state == 1 && startGame == 0) { // selecting an option from the menu
      state = 2;
      if (menuCurrentItem == 0) {
        subMenuOption = 1;
        lcd.clear();
        lcd.print("<START GAME>");
        lcd.setCursor(0, 1);
        lcd.print("Press to start");
      }
      else if (menuCurrentItem == 1) {
        lcd.clear();
        lcd.print("<LEADERBOARD>");
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(leaderboardPos+1);
        lcd.print(".");
        lcd.print(highscoreNames[leaderboardPos]);
        lcd.print(" - ");
        lcd.print(highscores[leaderboardPos]);
        lcd.print(" pct");
        leaderboard = 1;
      }
      else if (menuCurrentItem == 2) {
        lcd.clear();
        lcd.print("<SETTINGS>");
        settings = 1;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(">");
        lcd.print(settingsOptions[settingsPos]);
      }
      else if (menuCurrentItem == 3) {
        lcd.clear();
        lcd.print("<HOW TO PLAY>");
        int letter = 0;
        while (letter <= strlen(messageHTP) - 16) {
          showLetters(0, letter, messageHTP);
          delay(300);
          ++letter;
        }
      }
      else if (menuCurrentItem == 4) {
        lcd.clear();
        lcd.print("<ABOUT>    SNAKE");
        int letter = 0;
        while (letter <= strlen(messageAbout) - 16) {
          showLetters(0, letter, messageAbout);
          delay(300);
          ++letter;
        }
      }
      joyBackToMiddleY = HIGH;
    }
    else if (yValue < minThreshold && joyBackToMiddleY == LOW && state == 2 && startGame == 0) { // moving back to the main menu
      state = 1;
      leaderboardPos = 0;
      subMenuOption = 0;
      leaderboard = 0;
      settingsPos = 0;
      settings = 0;
      lcd.clear();
      lcd.print("<MENU>");          
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(menuOptions[menuCurrentItem]);
      lc.clearDisplay(0);
      for (int row = 0; row < matrixSize; row++) {
        lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
      }
      joyBackToMiddleY = HIGH;
    }
    else if (yValue > maxThreshold && joyBackToMiddleY == LOW && state == 2 && startGame == 0 && settings == 1) { // choosing an option from the settings submenu
      state = 3;
      lcd.clear();
      if (settingsPos == 0) {
        lcd.print("<NAME>  Press to");
        lcd.setCursor(0, 1);
        lcd.print("    ");
        lcd.print(nameLetters[0]);
        lcd.print(nameLetters[1]);
        lcd.print(nameLetters[2]);
        lcd.print("     SAVE");
      }
      else if (settingsPos == 1) {
        lcd.print("<LCD BRIGHT.>");
        lcd.setCursor(0, 1);
        lcd.print("Level: ");
        lcd.print(LCDbrightness);
      }
      else if (settingsPos == 2) {
        for(int row =0; row < matrixSize; row++) {
          for(int col =0; col < matrixSize; col++) {
            lc.setLed(0, row, col, true);
          }
        }
        lcd.print("<MATRIX BRIGHT.>");
        lcd.setCursor(0, 1);
        lcd.print("Level: ");
        lcd.print(matrixBrightness);
      }
      else if (settingsPos == 3) {
        lcd.print("<DIFFICULTY>");
        lcd.setCursor(0, 1);
        lcd.print("Level: ");
        lcd.print(difficulty);
      }
      else if (settingsPos == 4) {
        lcd.print("<SOUNDS>");
        lcd.setCursor(0, 1);
        lcd.print("Value: ");
        if (sounds == 1) 
          lcd.print("ON");
        else
          lcd.print("OFF");
      }
      joyBackToMiddleY = HIGH;
    }
     else if (yValue < minThreshold && joyBackToMiddleY == LOW && state == 3 && startGame == 0 && settings == 1 and settingsPos != 0) { // going back to the settings submenu
      state = 2;
      lcd.clear();
      lcd.print("<SETTINGS>");
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(settingsOptions[settingsPos]);
      lc.clearDisplay(0);
        for (int row = 0; row < matrixSize; row++) {
          lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
        }
      joyBackToMiddleY = HIGH;
    }
    else if (yValue < minThreshold && joyBackToMiddleY == LOW && state == 3 && startGame == 0 && settings == 1 and settingsPos == 0) { // moving left through the name field
      if (namePos > 0) {
        lcd.setCursor(namePos + 4, 1);
        lcd.print(nameLetters[namePos]);
        namePos--;
      }
      joyBackToMiddleY = HIGH;
    }
    else if (yValue > maxThreshold && joyBackToMiddleY == LOW && state == 3 && startGame == 0 && settings == 1 and settingsPos == 0) { // moving right through the name field
      if (namePos < 2) {
        lcd.setCursor(namePos + 4, 1);
        lcd.print(nameLetters[namePos]);
        namePos++;
      }
      joyBackToMiddleY = HIGH;
    }
    else if (joyBackToMiddleY == HIGH && yValue < maxThreshold && yValue > minThreshold && startGame == 0) {
      joyBackToMiddleY = LOW;
    }
}

// function used for different uses of the x axis of the joystick
void xAxisLogic() {
  if (xValue < minThreshold && joyBackToMiddleX == LOW && state == 1 && startGame == 0 && leaderboard == 0) { // moving down through the menu
      if (menuCurrentItem < 4) {
        menuCurrentItem++;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(">");
        lcd.print(menuOptions[menuCurrentItem]);
        lc.clearDisplay(0);
        for (int row = 0; row < matrixSize; row++) {
          lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
        }
      }
      joyBackToMiddleX = HIGH;
    }
    else if (xValue > maxThreshold && joyBackToMiddleX == LOW && state == 1 && startGame == 0 && leaderboard == 0) { // moving up through the menu
      if (menuCurrentItem > 0) {
        menuCurrentItem--;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(">");
        lcd.print(menuOptions[menuCurrentItem]);
        lc.clearDisplay(0);
        for (int row = 0; row < matrixSize; row++) {
          lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
        }
      }
      joyBackToMiddleX = HIGH;
    }
    else if (xValue < minThreshold && joyBackToMiddleX == LOW && state == 2 && startGame == 0 && leaderboard == 1) { // moving down through the leaderboard
      if (leaderboardPos < 4) {
        leaderboardPos++;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(leaderboardPos+1);
        lcd.print(".");
        lcd.print(highscoreNames[leaderboardPos]);
        lcd.print(" - ");
        lcd.print(highscores[leaderboardPos]);
        lcd.print(" pct");
      }
      joyBackToMiddleX = HIGH;
    }
    else if (xValue > maxThreshold && joyBackToMiddleX == LOW && state == 2 && startGame == 0 && leaderboard == 1) { // moving up through the leaderboard
      if (leaderboardPos > 0) {
        leaderboardPos--;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(leaderboardPos+1);
        lcd.print(".");
        lcd.print(highscoreNames[leaderboardPos]);
        lcd.print(" - ");
        lcd.print(highscores[leaderboardPos]);
        lcd.print(" pct");
      }
      joyBackToMiddleX = HIGH;
    }
    else if (xValue < minThreshold && joyBackToMiddleX == LOW && state == 2 && startGame == 0 && settings == 1) { // moving down through the settings submenu
      if (settingsPos < 4) {
        settingsPos++;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(">");
        lcd.print(settingsOptions[settingsPos]);
      }
      joyBackToMiddleX = HIGH;
    }
    else if (xValue > maxThreshold && joyBackToMiddleX == LOW && state == 2 && startGame == 0 && settings == 1) { // moving up through the settings submenu
      if (settingsPos > 0) {
        settingsPos--;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(">");
        lcd.print(settingsOptions[settingsPos]);
      }
      joyBackToMiddleX = HIGH;
    }
    else if (xValue < minThreshold && joyBackToMiddleX == LOW && state == 3 && startGame == 0 && settings == 1) { // different values when in the settings submenu
      if (settingsPos == 0) { // scrolling through letters while in the input of the name
        if (nameLetters[namePos] > 'A') {
          nameLetters[namePos]--;
          lcd.setCursor(namePos + 4, 1);
          lcd.print(nameLetters[namePos]);
        }
      }
      else if (settingsPos == 1) { // changing the brightness
        if (LCDbrightness > 1) {
          LCDbrightness--;
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print("Level: ");
          lcd.print(LCDbrightness);
          analogWrite(3, LCDbrightness * 51);
          EEPROM.update(1, LCDbrightness);
        }
      }
      else if (settingsPos == 2) { // changing the brightness
        if (matrixBrightness > 1) {
          matrixBrightness--;
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print("Level: ");
          lcd.print(matrixBrightness);
          lc.setIntensity(0, 3 * matrixBrightness);
          EEPROM.update(2, matrixBrightness);
        }
      }
      else if (settingsPos == 3) { // changing the difficulty
        if (difficulty > 1) {
          difficulty--;
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print("Level: ");
          lcd.print(difficulty);
          EEPROM.update(3, difficulty);
        }
      }
      else if (settingsPos == 4) { // changing the sounds
        if (sounds == 1) {
          sounds = 0;
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print("Value: ");
          lcd.print("OFF");
          EEPROM.update(4, sounds);
        }
      }
      joyBackToMiddleX = HIGH;
    }
    else if (xValue > maxThreshold && joyBackToMiddleX == LOW && state == 3 && startGame == 0 && settings == 1) { // different values when in the settings submenu
      if (settingsPos == 0) {
        if (nameLetters[namePos] < 'Z') {
          nameLetters[namePos]++;
          lcd.setCursor(namePos + 4, 1);
          lcd.print(nameLetters[namePos]);
        }
      }
      else if (settingsPos == 1) {
        if (LCDbrightness < 5) {
          LCDbrightness++;
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print("Level: ");
          lcd.print(LCDbrightness);
          analogWrite(3, LCDbrightness * 51);
          EEPROM.update(1, LCDbrightness);
        }
      }
      else if (settingsPos == 2) {
        if (matrixBrightness < 5) {
          matrixBrightness++;
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print("Level: ");
          lcd.print(matrixBrightness);
          lc.setIntensity(0, 3 * matrixBrightness);
          EEPROM.update(2, matrixBrightness);
        }
      }
      else if (settingsPos == 3) {
        if (difficulty < 3) {
          difficulty++;
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print("Level: ");
          lcd.print(difficulty);
          EEPROM.update(3, difficulty);
        }
      }
      else if (settingsPos == 4) {
        if (sounds == 0) {
          sounds = 1;
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print("Value: ");
          lcd.print("ON");
          EEPROM.update(4, sounds);
        }
      }
      joyBackToMiddleX = HIGH;
    }
    else if (joyBackToMiddleX == HIGH && xValue < maxThreshold && xValue > minThreshold && startGame == 0) {
      joyBackToMiddleX = LOW;
    }
}

void setup() {
  putInitialHStoEEPROM();
  LCDbrightness = EEPROM.read(1);
  matrixBrightness = EEPROM.read(2);
  difficulty = EEPROM.read(3);
  sounds = EEPROM.read(4);
  //song();
  analogWrite(3, LCDbrightness * 51);
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 3 * matrixBrightness);
  lc.clearDisplay(0);
  for (int row = 0; row < matrixSize; row++) {
    lc.setRow(0, row, matrixHello[row]);
  }
  randomSeed(analogRead(A2));
  lcd.begin(16, 2);
  lcd.print("     SNAKE!");
  lcd.setCursor(0, 1);
  lcd.print(" Press to begin");
  getHighscores();
}

void loop() {
  if (startGame == 1) {
    game();
    if (score == 10) {
      String newName = "";
      newName += nameLetters[0];
      newName += nameLetters[1];
      newName += nameLetters[2];
      updateHStoEEPROM(score, newName); 
      getHighscores();
      exitGame();
    }
  }
  else {
    blinkLetterName();
    swState = digitalRead(pinSW);
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    buttonLogic();
    yAxisLogic();
    xAxisLogic();
  }
}

// function used for printing the scrolling text
void showLetters(int printStart, int startLetter, char* message) {
  lcd.setCursor(printStart, 1);
  for (int letter = startLetter; letter <= startLetter + 15; letter++) {
    lcd.print(message[letter]);
  }
  lcd.print(" ");
}

// function used for the game logic
void game() {
  if (millis() - lastBlink > blinkInterval) {
    matrix[currentFoodRow][currentFoodColumn] = !matrix[currentFoodRow][currentFoodColumn];
    updateMatrix();
    lastBlink = millis();
  }
  if (millis() - lastMoved > moveInterval) {
    updatePositions();
    lastMoved = millis();
  }
  if (matrixChanged == true) {
    updateMatrix();
    matrixChanged = false;
  }
}

// function that generates new food (blinking point on the matrix)
void generateFood() {
  lastFoodRow = currentFoodRow;
  lastFoodColumn = currentFoodColumn;
  currentFoodRow = random(0, 8);
  currentFoodColumn = random(0, 8);
  matrix[lastFoodRow][lastFoodColumn] = 0;
  matrix[currentFoodRow][currentFoodColumn] = 1;
  matrixChanged = true;
}

// function that updates the matrix
void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

// function that updates the positions according to the movement
// of the joystick
void updatePositions() {
  int xValue = analogRead(pinX);
  int yValue = analogRead(pinY);
  
  xLastPos = xPos;
  yLastPos = yPos;
  if (xValue < minThreshold) {
    if (xPos < matrixSize - 1) {
      xPos++;
    } 
    else {
      xPos = 0;
    }
  }
  if (xValue > maxThreshold) {
    if (xPos > 0) {
      xPos--;
    }
    else {
      xPos = matrixSize - 1;
    }
  }

  if (yValue > maxThreshold) {
    if (yPos < matrixSize - 1) {
      yPos++;
    } 
    else {
      yPos = 0;
    }
  }

  if (yValue < minThreshold) {
    if (yPos > 0) {
      yPos--;
    }
    else {
      yPos = matrixSize - 1;
    }
  }
  if (xPos != xLastPos || yPos != yLastPos) {
     if (xPos == currentFoodRow && yPos == currentFoodColumn) {
      score++;
      lcd.setCursor(7, 1);
      lcd.print(score);
      generateFood();
      Serial.print("score is: ");
      Serial.println(score);
    }
    matrixChanged = true;
    matrix[xLastPos][yLastPos] = 0;
    matrix[xPos][yPos] = 1;
  }  
}

// function used after the game is finished
void exitGame() {
   for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, true); 
    }
  }
  lcd.clear();
  menuCurrentItem = 0;
  subMenuOption = 0;
  startGame = 0;
  lcd.print("Congrats, ");
  lcd.print(nameLetters[0]);
  lcd.print(nameLetters[1]);
  lcd.print(nameLetters[2]);
  lcd.print("!");
  lcd.setCursor(0, 1);
  lcd.print("Your score: ");
  lcd.print(score);
  score = 0;
  delay(1000);
  for (int row = 0; row < matrixSize; row++) {
      lc.setRow(0, row, matrixHappy[row]);
    }
  delay(4000);
  state = 1;
  lcd.clear();
  lcd.print("<MENU>");          
  lcd.setCursor(0, 1);
  lcd.print(">");
  lcd.print(menuOptions[menuCurrentItem]);
  lc.clearDisplay(0);
  for (int row = 0; row < matrixSize; row++) {
    lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
  }
}
