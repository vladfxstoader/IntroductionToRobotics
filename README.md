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
TODO
### Demo
TODO
### Code
TODO

</details>