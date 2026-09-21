---
layout: default
title: Arduino Snake Arcade — Innovator Journal
permalink: /arduino-arcade-journal/
---

[← Back to my homepage](https://MatthewMa11.github.io/)

# Arduino Snake Arcade — Innovator Journal

**Unit 1 Summative: Extend Your Circuit — Tech & Innovation**  
**Date:** September 20, 2026

## The skills I developed throughout this project

I built a Snake game using an Arduino, a joystick, and an 8 × 8 LED matrix. The joystick controls the snake's direction. This builds on analog input from class: the Arduino reads the joystick's two axes instead of one potentiometer. I also used digital outputs to control the LEDs.

I finished the wiring and code first, then designed and 3D-printed the arcade. I put the screen and Arduino inside. The two breadboards did not fit, so I left them outside.

## Components and wiring

| Component | Purpose |
| --- | --- |
| Arduino Uno | Runs the game |
| 1588BS LED matrix | Displays the game |
| Analog joystick | Controls the snake's direction |
| Two breadboards | Hold the circuit connections |
| 200 Ω and 220 Ω resistors | Limit current through the LEDs |
| Connecting wires | Connect the components |

### Matrix wiring

![Arduino Uno and 1588BS matrix wiring reference](https://MatthewMa11.github.io/assets/arduino-arcade/matrix-wiring-reference.png)

*Matrix wiring reference. The joystick connections are listed below.*

The diagram shows four 220 Ω resistors on D2–D5 and four 200 Ω resistors on A0–A3. These are the row connections. D6–D13 control the columns.

```cpp
const byte ROWS[8] = {2, 3, 4, 5, A3, A2, A1, A0};
const byte COLS[8] = {6, 7, 8, 9, 10, 11, 12, 13};
```

### Joystick wiring

| Joystick pin | Arduino connection |
| --- | --- |
| VRx | A4 |
| VRy | A5 |
| VCC | 5 V |
| GND | GND |
| SW | Not connected |

The joystick sends two analog signals to the Arduino. Each reading ranges from 0 to 1023, while a digital input is read as `HIGH` or `LOW`. [Arduino analogRead reference](https://github.com/arduino/reference-en/blob/master/Language/Functions/Analog%20IO/analogRead.adoc)

On a breadboard, holes in the same connected strip share an electrical connection. The power rails are separate from these strips. The wires connect the components, and the resistors limit LED current.

## Development process

| Stage | What I did |
| --- | --- |
| Wiring | Connected the circuit using two breadboards |
| Programming | Made Snake respond to the joystick |
| CAD | Designed the arcade enclosure |
| Assembly | Put the screen and Arduino inside |
| Fit | Left the breadboards outside because they did not fit |

### Building the arcade

The enclosure had room for the screen and Arduino, but not the breadboards. I left the breadboards outside and kept them connected.

![CAD preview of the arcade](https://MatthewMa11.github.io/assets/arduino-arcade/arcade_easy_install_preview.png)

*CAD view of the arcade.*

The model is about **100 × 110 × 145 mm**. It has an open back, a removable joystick panel, and a frame that holds the screen.

![Removable joystick panel](https://MatthewMa11.github.io/assets/arduino-arcade/arcade_joystick_exploded.png)

*The joystick panel can be installed separately.*

## Code explanation

[View the full code](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/arduino-arcade/snake_arcade.ino) · [Download the Arduino sketch](https://MatthewMa11.github.io/assets/arduino-arcade/snake_arcade.ino)

### Starting the game

`setup()` configures the matrix pins as outputs and starts Serial communication at **9600 baud**. It also seeds the random-number generator and starts a new game.

`startGame()` creates a three-segment snake facing right, resets the timers, and places food in an empty cell.

### Reading the joystick

`readJoystick()` reads A4 and A5:

```cpp
int x = analogRead(A4) - 512;
int y = analogRead(A5) - 512;
```

Subtracting 512 makes the center reading close to zero. `REVERSE_Y = true` flips the Y direction.

The code ignores input when both axes are less than **170** from center. This stops small changes near the center from turning the snake. For diagonal input, it uses the axis with the larger movement.

`requestDirection()` blocks a turn directly backward into the snake's body. It saves an accepted direction for the next move. The joystick is checked about every **20 ms**.

The joystick button is not connected. If a button were added, debouncing could prevent one press from being read several times. [Arduino debounce example](https://github.com/arduino/arduino-examples/blob/main/examples/02.Digital/Debounce/Debounce.ino)

### Moving the snake

`snakeX[64]` and `snakeY[64]` store the body coordinates. Index 0 is the head, and `snakeLength` tracks the number of segments.

Every **800 ms**, `moveSnake()` calculates the next head position. Hitting the edge or the body ends the game. Eating food adds one segment.

The code moves each body segment into the previous segment's old position, then updates the head. It allows the head to enter the old tail position when the tail is moving away.

### Food and score

`snakeAt()` checks whether a cell contains part of the snake. `placeFood()` randomly selects an empty cell.

The score is `snakeLength - 3`, since the snake starts with three segments. Scores appear in the Serial Monitor. Filling all **64 cells** wins the game.

### Displaying the game

`drawDisplay()` stores the screen pattern in `pixels[8][8]`. The snake stays solid, and the food blinks for **200 ms on and 200 ms off**. An X means game over; a square outline means a win.

`scanDisplay()` selects one LED at a time, with at least **100 microseconds** between scan steps. It turns off the previous selection before lighting the next one. Repeating this makes the pattern appear continuous. A selected row is `HIGH`, and a selected column is `LOW`.

### Timing and restart

| State | What happens |
| --- | --- |
| `READY` | Waits one second before movement starts |
| `RUNNING` | Reads the joystick and moves the snake |
| `LOST` | Shows an X, then restarts after three seconds |
| `WON` | Shows a square outline, then restarts after three seconds |

`loop()` uses `millis()` and `micros()` to track time. This lets the display keep updating while the program waits for the next move.

## Working demo

A photo or video of the actual game still needs to be added. The images above show the wiring reference and CAD model.

## Peer support

I helped **Blair Wen** with wiring his circuit. I also helped **Sean Wei** CAD his fan holder.

## Reflection

The joystick turns physical movement into numbers that the code uses to control the game.

It could also control a menu for someone who finds small buttons difficult to use. That version would need a way to confirm a selection and a clear display. I would use analog-input debugging to check the center readings and direction settings.

## References

- Wiring diagram shown above: original source link still to add.
- [My Snake code](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/arduino-arcade/snake_arcade.ino).
- [Arduino analogRead reference](https://github.com/arduino/reference-en/blob/master/Language/Functions/Analog%20IO/analogRead.adoc).
- [Arduino debounce example](https://github.com/arduino/arduino-examples/blob/main/examples/02.Digital/Debounce/Debounce.ino).

The Arduino references were added for the code explanation.
