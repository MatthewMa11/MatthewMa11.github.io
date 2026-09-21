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

I built a Snake game using an Arduino, a joystick, and an 8 × 8 LED matrix. I built on **analog input**, which we practised with a potentiometer. My new input component is the joystick: instead of reading one position, the Arduino reads its horizontal and vertical axes.

A joystick suits Snake because its movement maps to the four directions in the game. The readings are analog, but the code turns them into up, down, left, or right. This adds decisions such as ignoring small movements near the center and choosing one direction when the joystick moves diagonally.

The LED matrix also extends the digital-output work from class. Each LED is controlled with on/off signals, but the program selects positions across an 8 × 8 grid to draw a moving game.

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

The diagram shows four 220 Ω resistors on D2–D5 and four 200 Ω resistors on A0–A3. These are the row connections. D6–D13 control the columns. The eight row and eight column connections let the Arduino select any of the 64 LEDs.

The sketch treats the rows as anodes. To light an LED, it sets that row HIGH and its column LOW. Current passes through the row resistor and the selected LED to the LOW column pin. The resistor limits the current. All eight resistors stay in the circuit.

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

On a breadboard, holes in the same connected strip share an electrical connection. The center gap separates the strips on each side. Power rails run separately and can have breaks, so their connections need checking. The joystick's GND connects to Arduino GND to give its voltage readings a common reference.

A0–A3 are used as digital outputs for the matrix in this sketch. A4 and A5 remain available for the joystick's two analog signals.

## Development process

| Stage | What I did |
| --- | --- |
| Wiring | Connected the Arduino, matrix, joystick, and resistors using two breadboards |
| Programming | Programmed Snake so the joystick controls direction and the matrix displays the game |
| CAD | After the wiring and code were done, designed the arcade enclosure |
| Assembly | Put the screen and Arduino inside the arcade |
| Fit problem | The two breadboards did not fit, so I left them outside and kept the circuit connected |

### Building the arcade

The enclosure had room for the screen and Arduino, but not the breadboards. I left the breadboards outside and kept them connected.

![CAD preview of the arcade](https://MatthewMa11.github.io/assets/arduino-arcade/arcade_easy_install_preview.png)

*CAD view of the arcade.*

The model is about **100 × 110 × 145 mm**. It has an open back, a removable joystick panel, and a frame that holds the screen.

![Removable joystick panel](https://MatthewMa11.github.io/assets/arduino-arcade/arcade_joystick_exploded.png)

*The joystick panel can be installed separately.*

## Code explanation

[View the full code](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/arduino-arcade/snake_arcade.ino) · [Download the Arduino sketch](https://MatthewMa11.github.io/assets/arduino-arcade/snake_arcade.ino)

### Running the sketch in Arduino IDE

To run the saved code:

1. Save `snake_arcade.ino` inside a folder named `snake_arcade` and open it in Arduino IDE.
2. Connect the Uno with a USB data cable. Select **Arduino Uno** and its port in the IDE.
3. Click **Verify** to check compilation, then **Upload** to send the sketch to the board.
4. Open the Serial Monitor at **9600 baud** to see new-game, score, and game-over messages.

If uploading fails, check the selected board, port, and USB connection. These are reproduction steps; the game rules and expected behavior are explained below. [Arduino upload guide](https://support.arduino.cc/hc/en-us/articles/4733418441116-Upload-a-sketch-in-Arduino-IDE)

### Starting the game

`setup()` configures the matrix pins as outputs and starts Serial communication at **9600 baud**. It also seeds the random-number generator and starts a new game.

`startGame()` creates a three-segment snake facing right, resets the timers, and places food in an empty cell.

### Reading the joystick

`readJoystick()` reads A4 and A5:

```cpp
int x = analogRead(A4) - 512;
int y = analogRead(A5) - 512;
```

Subtracting 512 makes the center reading close to zero. `REVERSE_X = false` keeps the X direction unchanged, while `REVERSE_Y = true` flips the Y direction. These settings control how the joystick's physical orientation matches the screen.

The code ignores input when both axes are less than **170** from center. This area is called a **dead zone**. It stops small changes near the center from turning the snake. For diagonal input, it uses the axis with the larger movement.

For example, raw readings of X = 800 and Y = 520 become X = 288 and Y = −8 after centering and reversing Y. X is the stronger axis, so the code requests a right turn. The turn is accepted unless it would reverse directly into the snake's body.

`requestDirection()` blocks a turn directly backward into the snake's body. It saves an accepted direction for the next move. The joystick is checked about every **20 ms**.

The joystick's SW button is not connected, so this sketch does not read a digital pushbutton. A digital input gives a HIGH or LOW state. A mechanical button can briefly switch between those states as its contacts settle, making one press look like several. Debouncing accepts a change only after it stays stable for a short time. That would be useful if a restart or selection button were added. [Arduino debounce example](https://github.com/arduino/arduino-examples/blob/main/examples/02.Digital/Debounce/Debounce.ino)

### Moving the snake

`snakeX[64]` and `snakeY[64]` store the body coordinates. Index 0 is the head, and `snakeLength` tracks the number of segments.

Every **800 ms**, `moveSnake()` calculates the next head position by adding the direction to the current coordinates. For example, moving right from (3, 4) gives (4, 4). Hitting the edge or the body ends the game. Eating food adds one segment.

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

`loop()` uses `millis()` and `micros()` to track time. Joystick input, snake movement, and display scanning have separate timers. This lets the program check the controls and refresh the display between moves. Using a long `delay()` for the snake's movement would stop that work while the delay runs.

The movement speed stays at one step every 800 ms, regardless of how far the joystick is pushed. The joystick chooses direction, not speed.

## Working demo

A photo or video of the actual game still needs to be added. The images above show the wiring reference and CAD model.

## Peer support

I helped **Blair Wen** with wiring his circuit. I also helped **Sean Wei** CAD his fan holder.

## Reflection

This project connects three skills: wiring an input, interpreting its readings, and using digital outputs to show a response. The code needs rules for those readings: the dead zone ignores small center changes, the stronger-axis check handles diagonal input, and the reverse-turn rule stops an invalid move. Reading a value is only the first step; deciding what it should do is what makes the control usable.

One possible use is a simple menu controller for someone who finds several small buttons difficult to operate. The joystick could move a highlight between choices. That version would need a clear display, a way to confirm a choice, and adjustable sensitivity. The current 8 × 8 game display would need to be replaced or adapted to show recognizable choices. I would also check whether that person could comfortably grip and move the joystick.

The skill I would rely on most is **debugging analog input**. I would print the raw X and Y readings, check their center and range, and compare each movement with the response on screen. That would help me choose a suitable dead zone and direction settings. The current fixed center of 512 and threshold of 170 are a starting point; a different joystick or user could need different settings.

## References

- Wiring diagram shown above: original source link still to add.
- [My Snake code](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/arduino-arcade/snake_arcade.ino).
- [Arduino analogRead reference](https://github.com/arduino/reference-en/blob/master/Language/Functions/Analog%20IO/analogRead.adoc).
- [Arduino debounce example](https://github.com/arduino/arduino-examples/blob/main/examples/02.Digital/Debounce/Debounce.ino).
- [Arduino upload guide](https://support.arduino.cc/hc/en-us/articles/4733418441116-Upload-a-sketch-in-Arduino-IDE).

The Arduino references support the explanations in this journal. The original source used while building the circuit still needs to be identified.
