# Introduction to Robotics (2022 - 2023)
Introduction to Robotics laboratory homeworks, taken in the 3rd year (2022-2023) at the Faculty of Mathematics and Computer Science, University of Bucharest. 
Each homework includes requirements, implementation details, code and image files.


# Homework 1

<details>
<summary>See details</summary>
<br>
  
### Components
RBG LED (1 minimum), potentiometers (3 minimum), resistors and wires (per logic).
### Task
Use a separat potentiometer in controlling each of the color of the RGB led (Red, Green and Blue). The control must be done with digital electronics (aka the value of the potentiometer must be read with Arduino, and a mapped value must be written to each of the pins connected to the led).
### Setup
![Setup](https://raw.githubusercontent.com/vladfxstoader/IntroductionToRobotics/main/Homework1/setup.jpg)
### [Demo](https://www.youtube.com/watch?v=tbynfti3OEo)
### [Code](https://github.com/vladfxstoader/IntroductionToRobotics/blob/main/Homework1/Homework1.ino)

</details>


# Homework 2

<details>
<summary>See details</summary>
<br>
  
### Components
5 LEDs, 1 button, 1 buzzer, resistors and wires (per logic).
### Task
Building  the  traffic  lights  for  a  crosswalk. 2 LEDs will be used to represent the traffic lights for people (red and green) and 3 LEDs to represent the traffic lights for cars (red, yellow and green).

The system has the following states:

1. State 1 (default, reinstated after state 4 ends): green light for cars, red light for people, no sounds. Duration: indefinite, changed  by pressing the button.

2. State 2 (initiated by counting down 8 seconds after a button press): the light should be yellow for cars, red for people and no sounds. Duration: 3 seconds.

3. State 3 (initiated after state 2 ends): red for cars, green for people and a beeping sound from the buzzer at a constant interval. Duration: 8 seconds.

4. State 4 (initiated after state 3 ends): red for cars, blinking green for people and a beeping sound from the buzzer, at a constant interval, faster than the beeping in state 3. This state should last 4 seconds.

Pressing  the  button  in  any  state  other  than  state  1  should NOT yield any actions.
### Setup
![Setup](https://raw.githubusercontent.com/vladfxstoader/IntroductionToRobotics/main/Homework2/setup.jpg)
### [Demo](https://www.youtube.com/watch?v=SKbSxfOfX60)
### [Code](https://github.com/vladfxstoader/IntroductionToRobotics/blob/main/Homework2/Homework2.ino)

</details>


# Homework 3

<details>
<summary>See details</summary>
<br>
  
### Components
1 7-segment display, 1 joystick, resistors and wires (per logic).
### Task
The joystick will be used to control the position ofthe segment and 'draw' on the display. The movement between segments should be natural (meaning they should jump from the current position only to neighbors, but without passing through 'walls').

The system has the following states:
1. State 1 (default, but also initiated after a button press in State 2): Current position blinking. Can use the joystick to move from one position to neighbors. Short pressing the button toggles state 2. Long pressing the button in state 1 resets the entire display by turning all the segments OFF and moving the current position to the decimal point.
2. State 2 (initiated after a button press in State 1): The  current segment stops blinking, adopting the state of the segment before selection (ON or OFF). Toggling the Y axis should change  the segment state from ON to OFF or from OFF to ON. Clicking the joystick should save the segment state and exit back to state 1.

Long pressing the button to reset should only be available in state 1.

### Setup
![Setup](https://raw.githubusercontent.com/vladfxstoader/IntroductionToRobotics/main/Homework3/setup.jpg)
### [Demo](https://www.youtube.com/watch?v=EkUDkmLJTAA)
### [Code](https://github.com/vladfxstoader/IntroductionToRobotics/blob/main/Homework3/Homework3.ino)

</details>


# Homework 4

<details>
<summary>See details</summary>
<br>
  
### Components
1 joystick, 1 4 digit 7-segment display, a 74hc595 shift register.
### Task
The joystick should be used to move through the 4 digit 7-segment displays digits; the button will be pressed in orded to lock in on the current digitthe other axis should be used to increment or decrement the number. Holding button pressed should reset all the digit values and the current position to the first digit in the first state.

The system has the following states:
1. State 1: a joystick axis  can be used to cycle through the 4 digits; using the other axis does nothing. A blinking decimal point shows the current digit position. When pressing the button, the selected digit is locked in and the second state is entered.
2. State 2: in this state, the decimal point stays always on, no longer blinking and the axis can not longer be used to cycle through the 4 digits. Instead, using the other axis, the number on the current digit IN HEX can be incremented or decremented. Pressing the button again return to the previous state. When changing the number, it can be incremented for each joystick movement - it does not increment continuosly if the joystick is kept in one position.
3. Reset: toggled by long pressing the buttononly in the first state. When resetting, all the digits go back to 0 and the current position is set to the first (rightmost) digit, in the first state.

### Setup
![Setup](https://raw.githubusercontent.com/vladfxstoader/IntroductionToRobotics/main/Homework4/setup.jpg)
### [Demo](https://www.youtube.com/watch?v=2rY6zYWMR2M)
### [Code](https://github.com/vladfxstoader/IntroductionToRobotics/blob/main/Homework4/Homework4.ino)

</details>