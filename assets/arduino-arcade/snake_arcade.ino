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


// ---------------- SETTINGS ----------------


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


// ---------------- GAME VARIABLES ----------------


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


// ---------------- DISPLAY ----------------


void blankDisplay() {
 for (byte i = 0; i < 8; i++) {
   digitalWrite(ROWS[i], ROW_OFF);
   digitalWrite(COLS[i], COL_OFF);
 }
}


// Light one LED at a time, including during full-screen flashes.
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


// ---------------- SNAKE AND FOOD ----------------


bool snakeAt(int x, int y, byte count) {
 for (byte i = 0; i < count; i++) {
   if (snakeX[i] == x && snakeY[i] == y) {
     return true;
   }
 }


 return false;
}


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


// ---------------- LEVELS ----------------


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


void finishGame() {
 state = LOST;


 blankDisplay();
 Serial.println(F("GAME OVER!"));
 printStatus();


 stateTime = millis();
}


void beginLevelFlash() {
 state = LEVEL_FLASH;
 stateTime = millis();


 foodX = foodY = -1;
 blankDisplay();
}


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


// ---------------- JOYSTICK ----------------


void requestDirection(int x, int y) {
 // Prevent reversing directly into the snake's neck.
 if (x == -dirX && y == -dirY) return;


 nextDirX = x;
 nextDirY = y;
}


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


// ---------------- MOVEMENT ----------------


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


// ---------------- DRAW THE GAME ----------------


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


// ---------------- SETUP ----------------


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


// ---------------- MAIN LOOP ----------------


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



