---
layout: default
title: Arduino Snake Arcade — Innovator Journal
permalink: /arduino-arcade-journal/
---

[← Back to my homepage](https://MatthewMa11.github.io/)

# Arduino Snake Arcade — Innovator Journal

**Unit 1 Summative: Extend Your Circuit — Tech & Innovation**  
**Journal started:** September 20, 2026

## My project and the skill I extended

I built an Arduino Snake game controlled by a joystick, then made an arcade enclosure for it. My goal was to connect physical controls to a game on an LED matrix. This extends the **analog input** work from class: instead of using one potentiometer, I use the two axes of a joystick to choose a direction. It also extends digital output from controlling one LED to displaying a moving game on an 8 × 8 matrix.

I completed the wiring and programming first. After that, I designed the arcade in CAD and put the screen and Arduino inside. The two breadboards did not fit, so I left them outside the arcade. This journal records that sequence and explains the final sketch. It is a retrospective account; separate dates for the build sessions have not been recorded here.

## Components and wiring

| Component | Role in my project |
| --- | --- |
| Arduino Uno | Reads the joystick and runs the game and display code |
| 1588BS LED matrix | Displays the snake, food, and end-of-game symbols |
| Analog joystick | Controls movement through its X and Y signals |
| Two breadboards | Hold the electrical connections without soldering |
| 200 Ω and 220 Ω resistors | Limit current in the eight matrix row connections shown below |
| Connecting wires | Connect the components and breadboards to the Arduino |

### Matrix wiring reference

![Arduino Uno and 1588BS matrix wiring reference, showing four 220-ohm resistors on D2–D5 and four 200-ohm resistors on A0–A3](https://MatthewMa11.github.io/assets/arduino-arcade/matrix-wiring-reference.png)

*Supplied matrix wiring reference. It shows a 9 V battery and switch, but those are not confirmed parts of my build. It also leaves out the joystick and the physical breadboard layout. The Arduino symbol carries a TheEngineeringProjects.com watermark; the original page for the complete diagram has not been identified.*

The diagram shows **eight resistors**: four 220 Ω resistors on D2–D5 and four 200 Ω resistors on A0–A3. The sketch uses these eight Arduino pins as matrix row outputs. It uses D6–D13 as the column outputs, so a separate matrix-driver library is not needed by this sketch.

The row and column order in the code is:

```cpp
const byte ROWS[8] = {2, 3, 4, 5, A3, A2, A1, A0};
const byte COLS[8] = {6, 7, 8, 9, 10, 11, 12, 13};
```

These arrays map the program's row and column coordinates to Arduino pins. Their positions are logical screen coordinates, not the numbered physical leads of the matrix package. A0–A3 are configured as digital outputs here, even though they can also be used as analog inputs.

### Joystick connections from my sketch

| Joystick connection | Arduino connection |
| --- | --- |
| VRx — horizontal signal | A4 |
| VRy — vertical signal | A5 |
| VCC | 5 V |
| GND | GND |
| SW — pushbutton | Not connected |

The joystick provides two changing voltages. The Arduino measures them relative to ground and interprets them as directional input. Unlike a digital input, which is read as `HIGH` or `LOW`, an analog input provides a range of values. The Uno's normal `analogRead()` range is 0–1023. [Arduino analogRead reference](https://github.com/arduino/reference-en/blob/master/Language/Functions/Analog%20IO/analogRead.adoc)

On the breadboards, holes within the same connected strip share an electrical connection. The center gap separates the terminal strips, and the long power rails are separate connections that may contain breaks. The wires connect those groups to the Arduino and components. The shared ground provides a reference for the joystick readings; the row resistors limit current through the selected LED path.

## Development process: circuit, code, then enclosure

| Stage | What I did | Result or decision |
| --- | --- | --- |
| Wiring | Connected the Arduino, joystick, matrix, and resistors using two breadboards | Built the electronic circuit before the housing |
| Programming | Programmed Snake with joystick direction control | Connected player input to movement on the display |
| CAD | Designed an arcade enclosure after the wiring and code were done | Created a housing for the screen and Arduino |
| Assembly | Put the screen and Arduino inside | Found that the breadboards did not fit |
| Adaptation | Left the breadboards outside the arcade | Kept the circuit connected, while accepting an incomplete enclosure |

### What did not work, and what I changed

The enclosure did not fit the complete circuit. There was room for the screen and Arduino, but there was no space for the breadboards. I changed the assembly by leaving the breadboards outside instead of trying to force them into the arcade.

This addressed the immediate space problem, but part of the circuit remained exposed and the arcade was less self-contained. The limitation showed that the breadboards, wires, and connection space needed to be included in the CAD layout along with the main components.

For the next revision, I would measure the complete connected circuit first. I would include both breadboards, wire routing, connector access, and room for assembly in CAD. Then I could compare making a larger enclosure with transferring the tested circuit onto a smaller, more permanent board. Both options would need another fit check.

![CAD preview of the arcade enclosure](https://MatthewMa11.github.io/assets/arduino-arcade/arcade_easy_install_preview.png)

*CAD view, not a photo of the final working build. The external breadboards are not shown.*

The saved CAD is approximately **100 × 110 × 145 mm**. It includes an open back, a removable joystick panel, and a display retainer. The removable panel makes the joystick mounting screws easier to reach, but it does not solve the missing breadboard space.

![Exploded CAD view of the removable joystick panel](https://MatthewMa11.github.io/assets/arduino-arcade/arcade_joystick_exploded.png)

*The joystick panel can be assembled separately and lowered into the cabinet.*

## My code and how it works

[View my complete Arduino sketch on GitHub](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/arduino-arcade/snake_arcade.ino) · [Download the sketch](https://MatthewMa11.github.io/assets/arduino-arcade/snake_arcade.ino)

The linked file preserves the supplied sketch unchanged. To open it as an Arduino project, save it as `snake_arcade.ino` inside a folder named `snake_arcade`. The explanation below describes the code; it is not a claim that every game condition has been demonstrated on hardware.

### 1. Starting the game

`setup()` sets all 16 matrix connections to their off levels, then configures them as outputs. It starts Serial communication at **9600 baud**, seeds the random-number generator with timing and joystick readings, and calls `startGame()` and `drawDisplay()`.

`startGame()` resets the snake to three segments at coordinates **(3, 4), (2, 4), and (1, 4)**, facing right. It resets the timers and score, places food in an empty cell, and enters the `READY` state. Although `foodX` and `foodY` have initial values near the top of the file, `placeFood()` chooses the food location when each game starts.

### 2. Reading the joystick and choosing a direction

`readJoystick()` reads A4 and A5 and subtracts 512 from each reading:

```cpp
int x = analogRead(A4) - 512;
int y = analogRead(A5) - 512;
```

This makes the expected center close to zero. `REVERSE_X` is `false`, while `REVERSE_Y` is `true`, so the code flips the Y reading to match the intended screen direction. These constants allow the input orientation to be adjusted without rewriting the movement code.

The function ignores input when **both axes are less than 170 units from center**. This dead zone prevents small resting variations from requesting a turn. When the joystick is tilted diagonally, it chooses the axis with the larger movement; a tie selects the horizontal axis.

`requestDirection()` rejects a direction directly opposite the snake's current movement. For example, a snake moving right cannot immediately reverse left into its neck. Accepted input goes into `nextDirX` and `nextDirY`; `moveSnake()` applies it at the next movement step. These variables hold the latest accepted request, rather than a queue of several turns.

The joystick is checked about every **20 ms** while the game is ready or running. Its pushbutton is not connected, so this sketch does not use `digitalRead()` or button debouncing. Debouncing would matter if a mechanical button were added: one press can produce several rapid electrical changes. The analog dead zone handles a different issue—variation around the joystick's center. [Arduino debounce example](https://github.com/arduino/arduino-examples/blob/main/examples/02.Digital/Debounce/Debounce.ino)

### 3. Storing and moving the snake

`snakeX[64]` and `snakeY[64]` store the coordinates of up to 64 segments, enough to fill the 8 × 8 board. Index 0 is the head. `snakeLength` says how many entries are currently used.

Every **800 ms**, `moveSnake()` applies the requested direction and calculates the next head position by adding `dirX` and `dirY`. It checks the move before changing the body:

- A position outside 0–7 on either axis ends the game. The snake does not wrap around the screen.
- Moving onto the snake's body ends the game.
- Moving onto food increases the length by one.

There is a useful detail in the collision check: when the snake is not eating, the old tail cell is excluded because the tail moves away during that step. When it is eating, the tail stays, so that cell remains part of the collision check.

The body is updated from the tail toward the head, copying each segment into the preceding segment's old position. Finally, the new head coordinates are stored at index 0. Increasing `MOVE_MS` slows the snake down; decreasing it speeds the snake up.

### 4. Food, score, and winning

`snakeAt()` checks whether a coordinate is occupied by the snake. `placeFood()` counts the empty cells, randomly selects one of them, and walks through the board until it reaches that unoccupied cell. This keeps food off the snake and avoids repeatedly guessing occupied positions.

The score is **`snakeLength - 3`**, since the snake starts with three segments. The program prints score updates in the Serial Monitor. Filling all **64 cells** calls `finishGame(true)` and wins the game; hitting a wall or the body calls `finishGame(false)`.

### 5. Drawing the LED matrix

`pixels[8][8]` is the picture the code wants to display. `drawDisplay()` clears that picture and fills it according to the game state. The snake is steady, while the food blinks for **200 ms on and 200 ms off**. A loss displays an X; a win displays a square outline.

`scanDisplay()` handles the physical LEDs. It steps through the 64 positions, waiting at least **100 microseconds** between scan steps. It blanks the old selection first, then lights the new position only if its entry in `pixels` is true. With `ROWS_ARE_ANODES = true`, a selected row is `HIGH` and a selected column is `LOW`.

Only one LED is selected at a time. Repeating the scan makes the pattern appear continuous, while the eight row resistors remain part of the current path. The scan interval is a minimum in the code; the actual refresh rate also depends on how quickly the loop executes.

### 6. Timing and automatic restart

| State | Behavior |
| --- | --- |
| `READY` | Shows the starting board and accepts direction input for one second |
| `RUNNING` | Reads the joystick and moves the snake every 800 ms |
| `LOST` | Shows an X for three seconds, then starts a new game |
| `WON` | Shows a square outline for three seconds, then starts a new game |

`loop()` uses elapsed times from `millis()` and `micros()` instead of a long `delay()`. The display can therefore continue scanning while the game waits for the next movement, input check, or restart. It calls `scanDisplay()` both before and after the game-update work.

## Final result and testing evidence

My result is a Snake game whose direction is controlled by the joystick. The screen and Arduino are installed inside the arcade, while both breadboards remain outside because they do not fit.

The code, matrix wiring reference, joystick connection table, and CAD images are included here. **A photo or short video of the actual working circuit still needs to be added** to satisfy the assignment's working-evidence requirement. The reference diagram is not a photo of my build.

The next evidence should show the external breadboards and demonstrate the joystick changing the snake's direction. Additional useful checks are food collection, a wall collision, and the automatic restart. These are checks to record, not completed test results claimed by this journal.

## Peer support

I helped **Blair Wen** with wiring his circuit. I also helped **Sean Wei** with CAD work for his fan holder. These contributions relate to two parts of my own project: building electrical connections and designing a part to hold hardware.

To make this record more specific, I still need to describe one exact wiring or CAD change I helped make, what the classmate needed, and what improved afterward. That detail will show how the support affected the work instead of only listing who I helped.

## Reflection and a possible real-world application

The project connects a physical input, code, and visual feedback. Its clearest design limitation is the enclosure: it holds the Arduino and screen but leaves out the breadboards that connect the circuit. The change I would make to my process is to measure and model the whole connected assembly before deciding on the enclosure dimensions.

One possible use for the joystick input would be a simple menu controller for someone who finds small, closely spaced buttons difficult to use. Moving the joystick could select an option, and a display could show the selection. This is a possible extension, not an application that the current arcade has been tested for.

To make it useful, I would replace Snake with a menu, add a clear way to confirm a selection, and provide readable feedback. I would also need a comfortable control, calibration for the user's movement, and a stable enclosure containing all the electronics. Feedback from the intended user would help determine whether it was easier to operate.

The skill I would rely on most is **debugging analog input**. Checking the wiring and inspecting the joystick readings would help distinguish a connection problem from an incorrect threshold or direction setting. In this sketch, the center offset, dead zone, and axis-reversal constants are specific places to investigate.

## Research and references

- **Supplied Arduino Uno + 1588BS wiring drawing:** reproduced above as the matrix reference, with the separate joystick connections taken from the sketch. The original tutorial or diagram URL still needs to be recorded; a watermark on a component symbol does not establish the source of the whole drawing.
- **My supplied Snake sketch:** linked in the code section and preserved unchanged. It is the source for the explanation of the pin assignments, functions, game rules, and timing.
- [Arduino analogRead reference](https://github.com/arduino/reference-en/blob/master/Language/Functions/Analog%20IO/analogRead.adoc): supports the explanation of analog input and its normal reading range.
- [Arduino debounce example](https://github.com/arduino/arduino-examples/blob/main/examples/02.Digital/Debounce/Debounce.ino): explains switch debouncing, which would be relevant if a button were added.
- [CAD dimensions and interference checks](https://MatthewMa11.github.io/assets/arduino-arcade/easy_joystick_verification.json) and [STL mesh checks](https://MatthewMa11.github.io/assets/arduino-arcade/easy_install_mesh_verification.json): earlier digital design records. These do not override the later physical finding that the breadboards did not fit.

The Arduino reference links were added while explaining the project. The original research source used during the build should be identified separately rather than treating newly added references as a record of earlier research.
