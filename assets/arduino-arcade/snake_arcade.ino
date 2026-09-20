#include <Arduino.h>


// Arduino Uno + 1588BS
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


// Corrected joystick directions.
const bool REVERSE_X = false;
const bool REVERSE_Y = true;


// Larger number = slower snake.
const unsigned long MOVE_MS = 800;


byte snakeX[64];
byte snakeY[64];
byte snakeLength = 3;


int dirX = 1;
int dirY = 0;
int nextDirX = 1;
int nextDirY = 0;


int foodX = 6;
int foodY = 4;


bool pixels[8][8];


enum GameState {
 READY,
 RUNNING,
 LOST,
 WON
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


// Light one LED at a time to limit pin current.
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


void placeFood() {
 int emptyCells = 64 - snakeLength;


 if (emptyCells == 0) {
   foodX = foodY = -1;
   return;
 }


 // Randomly select one unoccupied square.
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


void finishGame(bool win) {
 state = win ? WON : LOST;
 stateTime = millis();


 blankDisplay();


 if (win) {
   Serial.print(F("YOU WIN! Score: "));
 } else {
   Serial.print(F("GAME OVER! Score: "));
 }


 Serial.println(snakeLength - 3);
}


void startGame() {
 snakeLength = 3;


 // Start with a horizontal snake moving right.
 for (byte i = 0; i < snakeLength; i++) {
   snakeX[i] = 3 - i;
   snakeY[i] = 4;
 }


 dirX = nextDirX = 1;
 dirY = nextDirY = 0;


 state = READY;
 stateTime = lastMove = lastInput = millis();


 placeFood();
 blankDisplay();


 Serial.println(F("New game! Score: 0"));
}


// ---------------- JOYSTICK ----------------


void requestDirection(int x, int y) {
 // Prevent an immediate reverse into the snake's neck.
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


 // Hitting a screen edge ends the game.
 if (x < 0 || x > 7 || y < 0 || y > 7) {
   finishGame(false);
   return;
 }


 bool eating = x == foodX && y == foodY;


 // The tail moves away unless the snake eats.
 byte checkLength = snakeLength - (eating ? 0 : 1);


 if (snakeAt(x, y, checkLength)) {
   finishGame(false);
   return;
 }


 if (eating) {
   snakeLength++;
 }


 // Move each body segment into the previous one's place.
 for (int i = snakeLength - 1; i > 0; i--) {
   snakeX[i] = snakeX[i - 1];
   snakeY[i] = snakeY[i - 1];
 }


 snakeX[0] = x;
 snakeY[0] = y;


 if (eating) {
   if (snakeLength == 64) {
     finishGame(true);
     return;
   }


   placeFood();
   blankDisplay();


   Serial.print(F("Score: "));
   Serial.println(snakeLength - 3);
 }
}


// ---------------- DRAW THE GAME ----------------


void drawDisplay() {
 for (byte y = 0; y < 8; y++) {
   for (byte x = 0; x < 8; x++) {
     pixels[y][x] = false;
   }
 }


 if (state == LOST) {
   // An X means game over.
   for (byte i = 0; i < 8; i++) {
     pixels[i][i] = true;
     pixels[i][7 - i] = true;
   }


 } else if (state == WON) {
   // A square outline means you won.
   for (byte i = 0; i < 8; i++) {
     pixels[0][i] = true;
     pixels[7][i] = true;
     pixels[i][0] = true;
     pixels[i][7] = true;
   }


 } else {
   // Snake stays solid.
   for (byte i = 0; i < snakeLength; i++) {
     pixels[snakeY[i]][snakeX[i]] = true;
   }


   // Only the food blinks.
   if (millis() % 400 < 200) {
     pixels[foodY][foodX] = true;
   }
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


 randomSeed(
   micros() + analogRead(A4) + analogRead(A5)
 );


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
   if (now - stateTime >= 1000) {
     state = RUNNING;
     lastMove = now;
   }


 } else if (state == RUNNING) {
   if (now - lastMove >= MOVE_MS) {
     lastMove = now;
     moveSnake();
   }


 } else if (now - stateTime >= 3000) {
   startGame();
 }


 drawDisplay();
 scanDisplay();
}

