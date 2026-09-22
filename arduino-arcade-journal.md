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

I built a Snake game using an Arduino, a joystick, and an 8 × 8 LED matrix. I built on **analog input**, which we practiced with a potentiometer. My new input component is the joystick: instead of reading one position, the Arduino reads its horizontal and vertical axis.

A joystick suits Snake because its movement maps to the four directions in the game. The readings are analog, or in numbers, but the code turns them into up, down, left, or right. This also includes decisions such as ignoring small movements near the center and choosing a single direction when the joystick moves diagonally.

I finished the wiring and the first version of the code, then designed and 3D-printed the arcade. The screen fit inside, but the Arduino and both breadboards remained outside. The joystick mount did not print well enough for screws, so I used hot glue to attach the joystick. The latest code adds levels and increases speed.

I learned the most from coding, as I greatly developed my C++ programming skills by building the snake game. I learned more about how the Joystick and the matrix screen work, also how to program the game with clear logic.

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

The joystick sends two analog signals to the Arduino. Each reading ranges from 0 to 1023, while a digital input is read as `HIGH` or `LOW`. The joystick's GND connects to Arduino GND to give the power a way back. A0–A3 are used as outputs for the screen in this sketch. A4 and A5 remain available for the joystick's two analog signals.

## Development process

| Stage | What I did |
| --- | --- |
| Wiring | Connected the Arduino, matrix, joystick, and resistors using two breadboards |
| Programming | Programmed Snake so the joystick controls direction and the matrix displays the game |
| CAD | After the wiring and code were done, I CAD the arcade |
| Printing | Most of the print was usable; the joystick mount failed to print |
| Joystick assembly | With limited time, I used hot glue to attach the joystick |
| Fit problem | There is no space for the wires, so the Arduino could not fit; it stayed outside with both breadboards |
| Code revision | Added a level change after eating seven foods |

### Building the arcade

The 3D print failed, although most of the arcade printed well enough to use. The joystick mount did not print properly, so I could not install the joystick with screws as planned. With limited time, I attached it with hot glue.

I also forgot to account for the space the wires take up. Once the circuit was connected, I could not fit the Arduino inside the arcade. The screen fits perfectly into the 3D printed arcade, while the Arduino and both breadboards stayed outside.

There are two problems with the CAD arcade that I still need to fix: I will need to 3D print again, and its holes will need some slight adjustment. I also need to plan for space for the wire and the two breadboards.

![CAD preview of the arcade](https://MatthewMa11.github.io/assets/arduino-arcade/arcade_easy_install_preview.png)

The CAD model is about 100 × 110 × 145 mm. It has an open back, a removable joystick panel, and a frame for the screen. The planned joystick attachment differs from the hot-glued attachment used on the actual build.

![Removable joystick panel](https://MatthewMa11.github.io/assets/arduino-arcade/arcade_joystick_exploded.png)

## Code explanation

[View the full code](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/arduino-arcade/snake_arcade.ino) · [Download the Arduino sketch](https://MatthewMa11.github.io/assets/arduino-arcade/snake_arcade.ino)

### 1. Choose the pins and game settings

`#include <Arduino.h>` lets the program use Arduino commands such as `digitalWrite()` and `analogRead()`. `ROWS` and `COLS` list the pins connected to the eight rows and eight columns of the screen. To turn on one LED, the code sets its row to HIGH and its column to LOW. The OFF settings turn that connection off.

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

These settings control how the game works. A millisecond, written as **ms**, is one thousandth of a second.

| Setting | What it controls |
| --- | --- |
| `START_MOVE_MS = 800` | The snake starts by moving once every 800 ms, or 0.8 seconds |
| `SPEED_INCREASE_PERCENT = 15` | Each new level makes the snake move about 15% faster |
| `MIN_MOVE_MS = 80` | The time between moves cannot go below 80 ms |
| `FOODS_PER_LEVEL = 7` | Eating seven foods starts the next level |
| `FLASH_COUNT = 3` | The screen flashes three times between levels |
| `FLASH_PHASE_MS = 120` | Each flash stays on for 120 ms and off for 120 ms |
| `READY_MS = 1000` | Each level has a one-second pause before play |
| `GAME_OVER_MS = 3000` | The game-over screen stays for three seconds |

`const` means a setting stays the same while the program runs. Other values, such as `moveMs`, `level`, and `totalScore`, can change as the player plays.

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

### 2. Remember where everything is

`snakeX[64]` and `snakeY[64]` are two lists of numbers. Together, they record the position of each part of the snake. They have space for 64 parts because the screen has 64 squares. The first position in each list is numbered 0, and that is the head. `snakeLength` records how many parts the snake currently has.

`dirX` and `dirY` record where the snake is moving. For example, (1, 0) moves it one square right, and (0, −1) moves it one square up. `nextDirX` and `nextDirY` save the player’s new direction until it is time to move again.

`foodX` and `foodY` record where the food is. Setting them to −1 means there is no food on the screen. `pixels[8][8]` is an eight-by-eight list that tells the Arduino which LEDs should be on.

`foodsThisLevel` counts food eaten in the current level. `totalScore` counts food eaten during the whole game. These need to be separate because a new level resets the snake and the level’s food count, but keeps the total score.

`state` tells the program what is happening: getting ready, playing, flashing between levels, or showing game over. `stateTime`, `lastMove`, and `lastInput` remember when things happened so the code knows when to do them again.

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

`setup()` runs once when the Arduino turns on or resets. It starts with the screen off and sets the screen pins to send signals out. `Serial.begin(9600)` lets the Arduino send text to the computer; the Serial Monitor should also be set to 9600 baud.

`randomSeed()` uses the current time and joystick readings as a starting number for choosing food positions. Then `setup()` starts the game and prepares the first picture.

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

`startGame()` puts the game back at level 1 with a score of 0. It clears the number of foods eaten in that level and sets the time between moves to 800 ms. It then resets the snake, places food, shows the starting information in the Serial Monitor, and sets the game to `READY`.

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

`resetSnake()` makes the snake three parts long. The head starts at (3, 4), followed by (2, 4) and (1, 4), so all three parts are on the same row. The snake starts facing right.

The game waits one second in `READY` before changing to `RUNNING`. It then waits the current time between moves before moving the snake for the first time.

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

### 4. Put food in an empty square

`snakeAt(x, y, count)` checks whether the snake is on a certain square. It looks through the first `count` parts of the snake. If it finds a matching position, it returns `true`, meaning yes. Otherwise, it returns `false`, meaning no.

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

`placeFood()` works out how many squares are empty by subtracting the snake’s length from 64. It chooses a random number from those empty spaces, then checks the screen row by row. It skips squares containing the snake and counts through the empty ones until it reaches the chosen square. That is where the food goes.

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

If there are no empty squares, the food position becomes −1. With the current game settings, the snake resets after every seven foods, so it normally moves to the next level before the screen could fill. This check also makes sure food is not placed inside the snake.

### 5. Read the joystick and choose a direction

The Arduino checks the joystick about every **20 ms**, or 0.02 seconds, while the game is getting ready or being played. A4 reads left and right movement, and A5 reads up and down movement.

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

The joystick readings go from 0 to 1023. Subtracting 512 makes a centered joystick read close to zero. `REVERSE_X = false` leaves left and right unchanged. `REVERSE_Y = true` reverses the up-and-down reading so it matches the direction on the screen.

The code checks how far each reading is from zero, whether it is positive or negative. If both are below **170**, it keeps the current direction. This small area around the center is called the **dead zone**. It stops tiny changes in the reading from turning the snake by accident.

If the joystick moves diagonally, the code chooses whichever direction has the bigger reading. If both are equal, it uses left or right. For example, X = 800 and Y = 520 become X = 288 and Y = −8 after the calculations. The left-and-right reading is bigger, so the code asks the snake to turn right.

`requestDirection()` checks that turn before saving it. If the snake is moving right, it cannot immediately turn left into its own body. Any allowed turn is saved for the next move. Letting go of the joystick does not stop the snake. It keeps moving in its current direction. The joystick chooses direction, and the level sets the speed.

```cpp
void requestDirection(int x, int y) {
 // Prevent reversing directly into the snake's neck.
 if (x == -dirX && y == -dirY) return;

 nextDirX = x;
 nextDirY = y;
}
```

The joystick’s SW button is not connected. If a button were added later, one press could produce several quick on/off signals while the metal contacts settle. **Debouncing** means waiting until the signal stays steady before counting the press. This helps the program count one press once. [Arduino debounce example](https://github.com/arduino/arduino-examples/blob/main/examples/02.Digital/Debounce/Debounce.ino)

### 6. Move the snake and check what it hits

When enough time has passed for the next move, `moveSnake()` does these steps:

1. Uses the direction the player chose.
2. Works out the head’s next position. Moving right from (3, 4), for example, gives (4, 4).
3. Ends the game if the head goes past the screen’s edge. The row and column numbers must stay between 0 and 7.
4. Checks whether the head is about to reach food.
5. Checks whether the head is about to hit the snake’s body.
6. Adds one part if food was eaten, moves the body parts into the positions ahead of them, and puts the head in its new position.

The body positions are copied starting at the tail and working toward the head. This keeps the old positions available until the next part has copied them.

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

This line decides how much of the body to check:

```cpp
byte checkLength = snakeLength - (eating ? 0 : 1);
```

`eating ? 0 : 1` means “use 0 if the snake is eating, or 1 if it is not.” When the snake is not eating, the tail moves away, so the head is allowed to enter the square where the tail was. The code leaves that old tail square out of the check. When the snake eats and grows, the tail stays in place, so that square must still be checked.

### 7. Count food and flash before the next level

Each time the snake eats, the code adds 1 to both `totalScore` and `foodsThisLevel`. If it has eaten fewer than seven foods in that level, the code puts new food on the screen and shows the updated information in the Serial Monitor.

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

Eating the seventh food runs `beginLevelFlash()`. It changes the game to `LEVEL_FLASH`, remembers when the flashing started, and sets the food position to −1. The snake stops moving while the flashes play.

```cpp
void beginLevelFlash() {
 state = LEVEL_FLASH;
 stateTime = millis();

 foodX = foodY = -1;
 blankDisplay();
}
```

`drawDisplay()` makes the screen look fully on for 120 ms, then off for 120 ms. It repeats this three times:

```text
3 × 2 × 120 ms = 720 ms
```

The flashes take **720 ms**, or 0.72 seconds, altogether. The LEDs are still turned on one at a time very quickly, even when the whole screen looks lit.

### 8. Start the next level and make the snake faster

After the flashes, `startNextLevel()` adds 1 to the level and resets the number of foods eaten in that level to 0. It then works out a shorter wait between moves.

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

The calculation divides the time between moves by **1.15**, which makes the snake move about 15% faster. It rounds the answer to the nearest whole millisecond. The `UL` after numbers such as `100UL` tells the Arduino to use a number type that can hold larger whole numbers during the calculation.

| Level | Time between moves |
| --- | --- |
| 1 | 800 ms |
| 2 | 696 ms |
| 3 | 605 ms |
| 4 | 526 ms |
| 18 onward | 80 ms |

A smaller wait makes the snake faster. The code stops the wait from going below **80 ms**, so it cannot keep speeding up forever. The level number can still increase after that.

The new level starts with a three-part snake, new food, and another one-second ready pause. The total score stays. For example, level 2 begins with a score of 7, even though the snake is three parts long again. There is no final win screen in this version.

### 9. Show the game on the LED screen

`drawDisplay()` decides which LEDs should be on and saves that picture in `pixels`:

- While getting ready or playing, the snake stays lit and the food blinks: 200 ms on, then 200 ms off.
- Between levels, the whole screen picture flashes on and off.
- After losing, two diagonal lines make an X.

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

Before drawing food, the code checks that its position is not −1. This stops it from trying to draw food outside the screen.

`scanDisplay()` turns the saved picture into lights on the real screen. It goes through the 64 LED positions one by one. Its `static` values remember which position it reached and when it last changed LEDs, so it can continue where it left off.

At least **100 microseconds**—one ten-thousandth of a second—must pass before it checks the next LED. `blankDisplay()` turns off the old LED first. The code then works out the next LED’s row and column and lights it if the saved picture says it should be on.

`index / 8` finds the row. `index % 8` gives the remainder after dividing by 8, which tells it the column. After the last LED, `% 64` brings the position back to 0. Repeating this quickly makes the picture appear on the screen.

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

### 10. Show game over and keep the game running

When the snake hits an edge or itself, `finishGame()` changes the game to `LOST`. It sends “GAME OVER!” and the final game information to the Serial Monitor, then starts the three-second wait. The screen shows an X during this time. Afterward, a new game starts from level 1 with score 0 and the starting speed.

```cpp
void finishGame() {
 state = LOST;

 blankDisplay();
 Serial.println(F("GAME OVER!"));
 printStatus();

 stateTime = millis();
}
```

`printStatus()` shows the level, total score, and time between moves in the computer’s Serial Monitor. The LED screen continues to show the game or its X symbol.

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

`loop()` runs over and over. It updates the LEDs, checks the joystick when the game allows it, decides what should happen next, and updates the screen again.

| Part of the game | What the code does |
| --- | --- |
| `READY` | Reads the joystick and waits one second before changing to `RUNNING` |
| `RUNNING` | Reads the joystick and moves the snake when the wait between moves is over |
| `LEVEL_FLASH` | Flashes for 720 ms, then starts the next level |
| `LOST` | Shows an X for three seconds, then starts a new game |

`millis()` tells the program how many milliseconds have passed since the Arduino started. `micros()` does the same in microseconds. The code compares these times with the times it saved earlier to decide when to act.

There is no long `delay()` that makes the whole program wait. This lets the Arduino keep updating the screen while waiting for the next move, the next level, or a new game.

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
