---
layout: default
title: Arduino Snake Arcade — Innovator Journal
permalink: /arduino-arcade-journal/
---

[← Back to my homepage](https://MatthewMa11.github.io/)

# Arduino Snake Arcade — Innovator Journal

**Unit 1 Summative: Extend Your Circuit — Tech & Innovation**  
**Date:** September 20, 2026  
**Updated:** September 21, 2026

## The skills I developed throughout this project

I built a Snake game using an Arduino, a joystick, and an 8 × 8 LED matrix. I built on **analog input**, which we practiced with a potentiometer. My new input component is the joystick: instead of reading one position, the Arduino reads its horizontal and vertical axes.

A joystick suits Snake because its movement maps to the four directions in the game. The readings are analog, or in numbers, but the code turns them into up, down, left, or right. This adds decisions such as ignoring small movements near the center and choosing one direction when the joystick moves diagonally.

The LED matrix also extends the digital-output work from class. Each LED is controlled with on/off signals, but the program selects positions across an 8 × 8 grid to draw a moving game.

I finished the wiring and the first version of the code, then designed and 3D-printed the arcade. The screen fit inside, but the Arduino and both breadboards remained outside. The joystick mount did not print well enough for screws, so I used hot glue to attach the joystick. The latest code adds levels and increases speed.

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

The diagram shows four 220 Ohm resistors on D2–D5 and four 200 Ohm resistors on A0–A3. These are the row connections. D6–D13 control the columns. The eight row and eight column connections let the Arduino select any of the 64 LEDs.

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

The joystick sends two analog signals to the Arduino. Each reading ranges from 0 to 1023, while a digital input is read as `HIGH` or `LOW`. The joystick's GND connects to Arduino GND to give the power a way back. A0–A3 are used as outputs for the matrix in this sketch. A4 and A5 remain available for the joystick's two analog signals.

## Development process

| Stage | What I did |
| --- | --- |
| Wiring | Connected the Arduino, matrix, joystick, and resistors using two breadboards |
| Programming | Programmed Snake so the joystick controls direction and the matrix displays the game |
| CAD | After the wiring and code were done, designed the arcade enclosure |
| Printing | Most of the print was usable, but the failed joystick mount could not hold the intended screws |
| Joystick assembly | With limited time, used hot glue to attach the joystick |
| Fit problem | Had not allowed space for the wires, so the Arduino could not fit; it stayed outside with both breadboards |
| Code revision | Added a level change after every seven foods, three flashes, and increasing speed |

### Building the arcade

The 3D print failed, although most of the arcade printed well enough to use. The joystick mount did not print properly, so I could not install the joystick with screws as planned. With limited time, I attached it with hot glue.

I also forgot to account for the space taken up by the wires. Once the circuit was connected, I could not fit the Arduino inside the arcade. The screen was inside, while the Arduino and both breadboards stayed outside.

There are two problems with the CAD arcade that I still need to fix: I will need to 3D print again, and its holes will need some slight adjustment. I also need to plan for space for the wire and the two breadboards.

![CAD preview of the arcade](https://MatthewMa11.github.io/assets/arduino-arcade/arcade_easy_install_preview.png)

The CAD model is about **100 × 110 × 145 mm**. It has an open back, a removable joystick panel, and a frame for the screen. The planned joystick attachment differs from the hot-glued attachment used on the actual build.

![Removable joystick panel](https://MatthewMa11.github.io/assets/arduino-arcade/arcade_joystick_exploded.png)

## Code explanation

[View the full code](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/arduino-arcade/snake_arcade.ino) · [Download the Arduino sketch](https://MatthewMa11.github.io/assets/arduino-arcade/snake_arcade.ino)

### 1. Set the pins and game settings

`#include <Arduino.h>` provides the Arduino functions and types used by the sketch. `ROWS` and `COLS` map screen coordinates to the matrix wiring listed above. With `ROWS_ARE_ANODES = true`, a selected row is HIGH and a selected column is LOW. The OFF values reverse those signals.

```cpp
#include <Arduino.h>

// Arduino Uno + 1588BS.
// Keep all eight matrix resistors installed.
//
// Joystick:
// VRx -> A4
// VRy -> A5
// VCC -> 5V
// GND -> GND
// SW  -> not connected

const byte ROWS[8] = {2, 3, 4, 5, A3, A2, A1, A0};
const byte COLS[8] = {6, 7, 8, 9, 10, 11, 12, 13};

const bool ROWS_ARE_ANODES = true;

const byte ROW_ON  = ROWS_ARE_ANODES ? HIGH : LOW;
const byte ROW_OFF = ROWS_ARE_ANODES ? LOW : HIGH;
const byte COL_ON  = ROWS_ARE_ANODES ? LOW : HIGH;
const byte COL_OFF = ROWS_ARE_ANODES ? HIGH : LOW;

const bool REVERSE_X = false;
const bool REVERSE_Y = true;
```

The settings at the top control the game:

| Setting | Meaning |
| --- | --- |
| `START_MOVE_MS = 800` | Start with 800 ms between moves |
| `SPEED_INCREASE_PERCENT = 15` | Increase movement speed by about 15% per level |
| `MIN_MOVE_MS = 80` | Never reduce the movement interval below 80 ms |
| `FOODS_PER_LEVEL = 7` | Advance after seven foods in a level |
| `FLASH_COUNT = 3` | Flash three times at a level change |
| `FLASH_PHASE_MS = 120` | Each flash has 120 ms on and 120 ms off |
| `READY_MS = 1000` | Wait one second before entering the running state |
| `GAME_OVER_MS = 3000` | Show game over for three seconds |

`const` settings stay fixed while the game runs. Variables such as `moveMs`, `level`, and `totalScore` change during play.

```cpp
// Starting time between movements.
// Larger number = slower snake.
const unsigned long START_MOVE_MS = 800;

// Speed increases by this percentage each level.
const unsigned int SPEED_INCREASE_PERCENT = 15;

// Fastest allowed time between movements.
const unsigned long MIN_MOVE_MS = 80;

// Eat 7 foods to advance to the next level.
const byte FOODS_PER_LEVEL = 7;

// Flash the screen three times.
const byte FLASH_COUNT = 3;

// Each flash: 120 milliseconds on, 120 milliseconds off.
const unsigned long FLASH_PHASE_MS = 120;

// Pause before starting each level.
const unsigned long READY_MS = 1000;

// Show game over for three seconds.
const unsigned long GAME_OVER_MS = 3000;
```

### 2. Store the snake, food, and game state

`snakeX[64]` and `snakeY[64]` hold the coordinates of up to 64 body segments. Index 0 is the head. `snakeLength` tells the code how many entries are currently used.

`dirX` and `dirY` store the current direction. For example, (1, 0) means right and (0, −1) means one row upward. `nextDirX` and `nextDirY` store the requested direction for the next move.

`foodX` and `foodY` locate the food. A value of −1 means no food is active. `pixels[8][8]` stores the image to display: each value says whether that LED should appear lit.

`foodsThisLevel` counts food in the current level. `totalScore` counts food across all levels in the current game. They are separate because the snake and level food count reset between levels, while the total score stays.

```cpp
byte snakeX[64];
byte snakeY[64];
byte snakeLength = 3;

int dirX = 1;
int dirY = 0;
int nextDirX = 1;
int nextDirY = 0;

int foodX = -1;
int foodY = -1;

bool pixels[8][8];

unsigned long level = 1;
unsigned long totalScore = 0;
byte foodsThisLevel = 0;

unsigned long moveMs = START_MOVE_MS;

enum GameState {
 READY,
 RUNNING,
 LEVEL_FLASH,
 LOST
};

GameState state = READY;

unsigned long stateTime = 0;
unsigned long lastMove = 0;
unsigned long lastInput = 0;
```

### 3. Start a new game

`setup()` runs once when the Arduino starts. It sets the matrix pins to their OFF values, configures them as outputs, and starts Serial communication at **9600 baud**. It seeds the random-number generator using the time and joystick readings, then calls `startGame()` and `drawDisplay()`.

```cpp
void setup() {
 for (byte i = 0; i < 8; i++) {
   digitalWrite(ROWS[i], ROW_OFF);
   digitalWrite(COLS[i], COL_OFF);

   pinMode(ROWS[i], OUTPUT);
   pinMode(COLS[i], OUTPUT);
 }

 Serial.begin(9600);

 randomSeed(micros() + analogRead(A4) + analogRead(A5));

 startGame();
 drawDisplay();
}
```

`startGame()` resets the level to 1, both food counters to 0, and the movement interval to 800 ms. It calls `resetSnake()`, `placeFood()`, and `printStatus()`, then enters `READY`.

```cpp
void startGame() {
 level = 1;
 totalScore = 0;
 foodsThisLevel = 0;
 moveMs = START_MOVE_MS;

 resetSnake();
 placeFood();
 printStatus();

 state = READY;
 stateTime = lastMove = lastInput = millis();
}
```

`resetSnake()` puts a three-segment snake at (3, 4), (2, 4), and (1, 4), facing right. After one second in `READY`, the game enters `RUNNING`. The first move happens one movement interval later.

```cpp
void resetSnake() {
 snakeLength = 3;

 // Start in the middle, moving right.
 for (byte i = 0; i < snakeLength; i++) {
   snakeX[i] = 3 - i;
   snakeY[i] = 4;
 }

 dirX = nextDirX = 1;
 dirY = nextDirY = 0;
}
```

### 4. Place food in an empty cell

`snakeAt(x, y, count)` checks whether a position matches any of the first `count` body segments.

```cpp
bool snakeAt(int x, int y, byte count) {
 for (byte i = 0; i < count; i++) {
   if (snakeX[i] == x && snakeY[i] == y) {
     return true;
   }
 }

 return false;
}
```

`placeFood()` calculates the number of empty cells, chooses a random number within that count, and walks through the grid. It skips cells occupied by the snake and places food at the selected empty cell. If no empty cells remain, it sets the food coordinates to −1.

```cpp
void placeFood() {
 int emptyCells = 64 - snakeLength;

 if (emptyCells <= 0) {
   foodX = foodY = -1;
   return;
 }

 int choice = random(emptyCells);

 for (byte y = 0; y < 8; y++) {
   for (byte x = 0; x < 8; x++) {
     if (!snakeAt(x, y, snakeLength)) {
       if (choice == 0) {
         foodX = x;
         foodY = y;
         return;
       }

       choice--;
     }
   }
 }
}
```

This avoids placing food inside the snake. In normal play, the next-level reset happens long before all 64 cells could fill.

### 5. Read the joystick and request a turn

While the game is `READY` or `RUNNING`, the joystick is checked about every **20 ms**:

```cpp
void readJoystick() {
 int x = analogRead(A4) - 512;
 int y = analogRead(A5) - 512;

 if (REVERSE_X) x = -x;
 if (REVERSE_Y) y = -y;

 int amountX = x < 0 ? -x : x;
 int amountY = y < 0 ? -y : y;

 // Ignore small movements near the center.
 if (amountX < 170 && amountY < 170) return;

 // Choose the stronger axis for diagonal input.
 if (amountX >= amountY) {
   requestDirection(x < 0 ? -1 : 1, 0);
 } else {
   requestDirection(0, y < 0 ? -1 : 1);
 }
}
```

Subtracting 512 puts the center near zero. `REVERSE_X = false` keeps X unchanged, and `REVERSE_Y = true` flips Y to match the screen direction.

The code finds the size of each axis reading, ignoring its sign. When both are below **170**, it returns without changing direction. This is the **dead zone**, which stops small center changes from causing turns.

For diagonal input, the larger axis determines the direction; X wins a tie. For example, raw X = 800 and Y = 520 become X = 288 and Y = −8 after centering and reversing Y, so the code requests a right turn.

`requestDirection()` rejects the exact opposite of the current direction. Otherwise, it saves the turn for the next move. Releasing the joystick does not stop the snake; it keeps its direction. The joystick controls direction, while the level controls speed.

```cpp
void requestDirection(int x, int y) {
 // Prevent reversing directly into the snake's neck.
 if (x == -dirX && y == -dirY) return;

 nextDirX = x;
 nextDirY = y;
}
```

The SW button is not connected, so this sketch does not read a digital pushbutton. Mechanical buttons can briefly switch HIGH and LOW as their contacts settle. Debouncing waits for a stable change so one press is not counted several times. That would matter if a restart or selection button were added. [Arduino debounce example](https://github.com/arduino/arduino-examples/blob/main/examples/02.Digital/Debounce/Debounce.ino)

### 6. Move the snake and check collisions

When the current movement interval has elapsed, `moveSnake()`:

1. Applies the requested direction.
2. Adds that direction to the head coordinates. Right from (3, 4), for example, gives (4, 4).
3. Ends the game if either coordinate is outside 0–7.
4. Checks whether the new head position contains food.
5. Checks for a collision with the body.
6. Adds a segment if food was eaten, shifts the body positions from tail to head, and writes the new head position.

```cpp
void moveSnake() {
 if (state != RUNNING) return;

 dirX = nextDirX;
 dirY = nextDirY;

 int x = snakeX[0] + dirX;
 int y = snakeY[0] + dirY;

 // Hitting an edge ends the game.
 if (x < 0 || x > 7 || y < 0 || y > 7) {
   finishGame();
   return;
 }

 bool eating = x == foodX && y == foodY;

 // The tail moves away unless the snake eats.
 byte checkLength = snakeLength - (eating ? 0 : 1);

 if (snakeAt(x, y, checkLength)) {
   finishGame();
   return;
 }

 if (eating && snakeLength < 64) {
   snakeLength++;
 }

 // Move each segment into the previous segment's position.
 for (int i = snakeLength - 1; i > 0; i--) {
   snakeX[i] = snakeX[i - 1];
   snakeY[i] = snakeY[i - 1];
 }

 snakeX[0] = x;
 snakeY[0] = y;

 if (eating) {
   totalScore++;
   foodsThisLevel++;

   // Every seven foods triggers the next level.
   if (foodsThisLevel >= FOODS_PER_LEVEL) {
     beginLevelFlash();
     return;
   }

   placeFood();
   printStatus();
 }
}
```

The body check uses:

```cpp
byte checkLength = snakeLength - (eating ? 0 : 1);
```

The expression `eating ? 0 : 1` means “use 0 when eating, otherwise use 1.” When the snake is not eating, the old tail moves away, so that cell is excluded from collision checking. When it grows, the tail stays and must be included.

### 7. Count food and flash between levels

After eating, the code increases both `totalScore` and `foodsThisLevel`. Before the seventh food, it places new food and prints the updated status.

```cpp
if (eating) {
   totalScore++;
   foodsThisLevel++;

   // Every seven foods triggers the next level.
   if (foodsThisLevel >= FOODS_PER_LEVEL) {
     beginLevelFlash();
     return;
   }

   placeFood();
   printStatus();
 }
```

The seventh food calls `beginLevelFlash()`. This switches to `LEVEL_FLASH`, records the start time, and removes the active food. The snake stops moving during this state.

```cpp
void beginLevelFlash() {
 state = LEVEL_FLASH;
 stateTime = millis();

 foodX = foodY = -1;
 blankDisplay();
}
```

`drawDisplay()` alternates the entire screen pattern on and off every 120 ms. Three flashes take:

```text
3 × 2 × 120 ms = 720 ms
```

The screen only appears fully lit: `scanDisplay()` still lights at most one LED at a time.

### 8. Start the next level and increase speed

After the flashes, `startNextLevel()` increases `level` and resets `foodsThisLevel`. It calculates the next movement interval:

```cpp
void startNextLevel() {
 level++;
 foodsThisLevel = 0;

 // Divide the movement interval by 1.15 for 15% more speed.
 // Round to the nearest whole millisecond.
 unsigned long divisor = 100UL + SPEED_INCREASE_PERCENT;

 moveMs = (moveMs * 100UL + divisor / 2) / divisor;

 if (moveMs < MIN_MOVE_MS) {
   moveMs = MIN_MOVE_MS;
 }

 // Reset the snake each level so the board cannot fill permanently.
 // Keep the total score and level.
 resetSnake();
 placeFood();
 printStatus();

 state = READY;
 stateTime = lastMove = lastInput = millis();
}
```

With a 15% increase, this divides the interval by **1.15** and rounds to the nearest whole millisecond. A shorter interval means more moves per second. The `UL` suffix makes the constants unsigned long values, keeping this arithmetic in a type large enough for the calculation.

| Level | Time between moves |
| --- | --- |
| 1 | 800 ms |
| 2 | 696 ms |
| 3 | 605 ms |
| 4 | 526 ms |
| 18 onward | 80 ms minimum |

The code clamps the interval at **80 ms**. Levels can keep increasing after that, but the movement speed stops increasing.

Each new level resets the snake to three segments, places food, prints the status, and returns to the one-second `READY` state. The total score is preserved. For example, level 2 starts with score 7 and a three-segment snake. There is no final win screen in this version.

### 9. Draw and refresh the LED matrix

`drawDisplay()` prepares the picture in `pixels`:

- During play or the ready pause, the snake is solid and the food blinks for 200 ms on and 200 ms off.
- During `LEVEL_FLASH`, every pixel alternates between on and off.
- During `LOST`, two diagonal lines form an X.

```cpp
void drawDisplay() {
 unsigned long now = millis();

 bool flashOn =
   state == LEVEL_FLASH &&
   ((now - stateTime) / FLASH_PHASE_MS) % 2 == 0;

 for (byte y = 0; y < 8; y++) {
   for (byte x = 0; x < 8; x++) {
     pixels[y][x] = flashOn;
   }
 }

 if (state == LEVEL_FLASH) return;

 if (state == LOST) {
   // Show an X after losing.
   for (byte i = 0; i < 8; i++) {
     pixels[i][i] = true;
     pixels[i][7 - i] = true;
   }

   return;
 }

 // Snake stays solid.
 for (byte i = 0; i < snakeLength; i++) {
   pixels[snakeY[i]][snakeX[i]] = true;
 }

 // Only food blinks.
 if (foodX >= 0 && foodY >= 0 && now % 400 < 200) {
   pixels[foodY][foodX] = true;
 }
}
```

The food-coordinate check prevents the code from using −1 as an array index when food is inactive.

`scanDisplay()` sends this picture to the hardware. Its `static` variables remember the current pixel and scan time between calls. At least **100 microseconds** must pass before it scans the next position. It blanks the previous LED, calculates `y = index / 8` and `x = index % 8`, and lights the selected LED if its pixel is true. `% 8` gives the remainder; `% 64` wraps the scan back to the start after the last cell.

```cpp
void blankDisplay() {
 for (byte i = 0; i < 8; i++) {
   digitalWrite(ROWS[i], ROW_OFF);
   digitalWrite(COLS[i], COL_OFF);
 }
}

void scanDisplay() {
 static byte index = 0;
 static unsigned long lastScan = 0;

 unsigned long now = micros();

 if (now - lastScan < 100) return;

 lastScan = now;
 blankDisplay();

 byte y = index / 8;
 byte x = index % 8;

 if (pixels[y][x]) {
   digitalWrite(COLS[x], COL_ON);
   digitalWrite(ROWS[y], ROW_ON);
 }

 index = (index + 1) % 64;
}
```

### 10. Handle game over and keep the tasks running

An edge or body collision calls `finishGame()`. It enters `LOST`, prints “GAME OVER!” and the final status, and starts a three-second timer. The display shows an X. After that, `startGame()` resets the score, level, speed, and snake for a new game.

```cpp
void finishGame() {
 state = LOST;

 blankDisplay();
 Serial.println(F("GAME OVER!"));
 printStatus();

 stateTime = millis();
}
```

`printStatus()` sends the level, total score, and movement interval to the Serial Monitor. Those numbers are not drawn on the 8 × 8 display.

```cpp
void printStatus() {
 blankDisplay();

 Serial.print(F("Level: "));
 Serial.print(level);

 Serial.print(F(" | Score: "));
 Serial.print(totalScore);

 Serial.print(F(" | Move: "));
 Serial.print(moveMs);
 Serial.println(F(" ms"));
}
```

`loop()` repeatedly scans the display, reads the joystick when allowed, handles the current state, redraws the picture, and scans again:

| State | What the main loop does |
| --- | --- |
| `READY` | Accepts direction input and waits one second before entering `RUNNING` |
| `RUNNING` | Reads the joystick and moves the snake when `moveMs` has elapsed |
| `LEVEL_FLASH` | Flashes for 720 ms, then starts the next level |
| `LOST` | Shows an X for three seconds, then starts a new game |

The timers use `millis()` for game events and `micros()` for scanning. There is no long `delay()`, so display scanning continues during the ready pause, level flashes, and game-over screen.

```cpp
void loop() {
 scanDisplay();

 unsigned long now = millis();

 if ((state == READY || state == RUNNING) &&
     now - lastInput >= 20) {
   lastInput = now;
   readJoystick();
 }

 if (state == READY) {
   if (now - stateTime >= READY_MS) {
     state = RUNNING;
     lastMove = now;
   }

 } else if (state == RUNNING) {
   if (now - lastMove >= moveMs) {
     lastMove = now;
     moveSnake();
   }

 } else if (state == LEVEL_FLASH) {
   if (now - stateTime >= FLASH_COUNT * 2UL * FLASH_PHASE_MS) {
     startNextLevel();
   }

 } else if (state == LOST && now - stateTime >= GAME_OVER_MS) {
   startGame();
 }

 drawDisplay();
 scanDisplay();
}
```

## Working demo

A photo or video of the actual game still needs to be added. The images above show the wiring reference and CAD model.

## Peer support

I helped **Blair Wen** with wiring his circuit. I also helped **Sean Wei** CAD his fan holder.

## Reflection

The final assembly differed from the CAD plan. A usable-looking enclosure still had a failed joystick mount, and I had left out the space needed by the wires. Hot glue let me attach the joystick within the time available, but it did not solve the Arduino fit problem. The build therefore kept the Arduino and breadboards outside.

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
