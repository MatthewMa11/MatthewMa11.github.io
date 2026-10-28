---
layout: default
title: Fight the Landlord Controller
permalink: /fight-the-landlord-journal/
---

[← Back to my homepage](/)

# Fight the Landlord Controller

Innovators Journal · October 27, 2026

[Open the game](/assets/fight-the-landlord/game.html) · [Download the project ZIP](/assets/fight-the-landlord/landlord-controller-project.zip) · [Download the Word journal](/assets/fight-the-landlord/fight-the-landlord-controller-journal.docx) · [View the Arduino code](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/fight-the-landlord/LandlordController/LandlordController.ino)

I turned the idea of a physical game controller into a working Arduino prototype for Fight the Landlord. I used a joystick, a separate push button, and three LEDs to connect physical actions to the browser game. The controller worked, and I think the results are quite good.

## Skill focus: turning ideas into reality

I can transform ideas into reality by managing my time effectively, following through on commitments, and maintaining momentum from concept to completed prototype or product.

### Managing my time

I set deadlines for myself, with the goal of completing the controller and getting it running before Monday’s class ended. This gave me a clear target for the project: a physical controller that could communicate with the game and respond to input.

I used ChatGPT to write the code and help me understand it. That support helped me move the project forward while learning how the Arduino and the browser worked together.

### Following through

I followed through on the goal of making a working physical controller. The project reached the point where the hardware worked with the game. I also provided photos, a demonstration video, and the project files, then documented the result in this GitHub journal.

I confirmed that my separate button uses Arduino pin 9. The shared sketch and documentation now match that connection. Checking this detail was part of making the finished project clear enough for someone else to understand and use.

### Maintaining momentum from idea to prototype

The project connects several pieces of work: the controller idea, the code, the physical hardware, and the demonstration. ChatGPT helped with the code and explanations, and I tried the physical controller and reported that it worked. The photos and video make the final prototype visible alongside the source files.

For me, the strongest evidence of this skill is the completed working prototype and its documentation. I brought the idea to a result that I could show, explain, and continue improving.

## How I used AI

### Using AI to write the code

I used ChatGPT to write the code for the browser game and its Arduino controller. The project needed two programs that could work together: one to read the physical controls and one to manage the cards, rules, and turns. AI helped produce the code that connects those parts. I am describing this openly because the code was AI-assisted, and that assistance was part of how I developed the prototype.

### Using AI to understand the code

I also used ChatGPT to help me understand the code. I asked for explanations by section, with the actual code shown together with each explanation. This makes it easier to follow what happens when I tilt the joystick, click it, or press the separate button. For example, readButton checks for a stable press, while handleHardwareInput turns its message into a game action. Looking at these sections together connects the physical controller to what happens on the screen.

### Using AI while keeping responsibility for the result

My target was to get the controller running before Monday’s class ended. ChatGPT gave me support with coding and explanations as I worked toward that target. I still needed a physical result to demonstrate, and I think the results are quite good because the controller worked. ChatGPT/Codex also helped organize this journal from my files and descriptions. I supplied the deadline, hardware information, photos, video, and my account of the result. The game itself runs the code in the browser and on the Arduino; it does not consult ChatGPT during play.

## How the prototype works

- Joystick left and right: moves the card highlight. Return it to center before moving again.
- Joystick click on D2: selects or deselects the highlighted card.
- Separate button on D9: plays selected cards or passes when allowed. During bidding, it chooses the next higher legal bid.
- LEDs on D4, D5, and D6: show my turn, Bot 1’s turn, and Bot 2’s turn.
- Screen buttons: connect the Arduino, choose bids, play or pass, request a hint, clear selections, restart, open the rules, and toggle sound.

The Arduino and computer communicate through a USB serial connection at 9600 baud. The Arduino sends short messages when I move or press a control. The browser checks the game rules and updates the cards. It sends turn messages back to the Arduino to choose which LED lights up. Connect Arduino lets me choose the board’s port.

## Detailed code walkthrough

The actual code appears directly above the explanation for each section. These excerpts show how the controller reads inputs, how the browser applies the game rules, and how the two programs exchange messages.


Each section below puts the original code excerpt first, followed by its explanation. The excerpts come directly from the shared project files. The Arduino file includes the confirmed pin 9 correction. The full files remain available through the source links; long functions are identified as excerpts when only part is shown.

The code is displayed as plain text without shaded boxes. Indentation and punctuation are kept so I can match each explanation to the actual source.

[Jump to the Arduino sketch](#arduino-pins) · [Jump to the browser game](#browser-visible-controls) · [Jump to the USB connection code](#serial-connect)

### Arduino sketch

<a id="arduino-pins"></a>

#### 1. Arduino: libraries, pins, and sensitivity

[Original LandlordController.ino, lines 14–32](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/LandlordController/LandlordController.ino#L14-L32)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">#include &lt;Arduino.h&gt;
#include &lt;string.h&gt;

const byte PIN_JOY_X = A4;
const byte PIN_JOY_SW = 2;
const byte PIN_PLAY_BTN = 9;
const byte LED_P0 = 4;
const byte LED_P1 = 5;
const byte LED_P2 = 6;

// Reversed to match the orientation of your joystick.
// Set false if you later rotate or replace the joystick.
const bool REVERSE_X = true;
const int CENTER_MIN = 400;
const int CENTER_MAX = 620;
const int THRESHOLD_LOW = 250;
const int THRESHOLD_HIGH = 770;
const unsigned long DEBOUNCE_DELAY = 50;
bool movedX = false;</pre>
{% endraw %}

Arduino.h supplies the Arduino functions, and string.h supplies strcmp for comparing incoming messages. The pin constants name each connection: the joystick horizontal signal uses A4, its click switch uses D2, the separate mini button uses D9, and the three turn LEDs use D4, D5, and D6. This is the current shared sketch, including my confirmed pin 9 correction.

The remaining constants tune the controls. A reading from 400 to 620 counts as centered. A reading below 250 or above 770 counts as a strong tilt. REVERSE_X reverses the direction for this joystick orientation. DEBOUNCE_DELAY sets a 50-millisecond wait for a stable button change. movedX starts false, meaning a new tilt is allowed.

<a id="arduino-memory"></a>

#### 2. Arduino: remembering button and message state

[Original LandlordController.ino, lines 34–46](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/LandlordController/LandlordController.ino#L34-L46)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">struct ButtonState {
  byte pin;
  bool lastReading;
  bool stableState;
  unsigned long changedAt;
};
ButtonState selectButton = {PIN_JOY_SW, HIGH, HIGH, 0};
ButtonState playButton = {PIN_PLAY_BTN, HIGH, HIGH, 0};

char incoming[24];
byte incomingLength = 0;
bool discardLine = false;
unsigned long lastTurnUpdate = 0;</pre>
{% endraw %}

ButtonState groups the information needed for one button: its pin, the most recent reading, the last accepted stable reading, and the time the reading changed. selectButton and playButton each have their own copy, so one button does not overwrite the other button’s timing. HIGH is the initial released state.

incoming holds a message from the computer. Its 24-character array can store up to 23 message characters plus the ending zero character used by C strings. incomingLength tracks the next position, discardLine marks an oversized message, and lastTurnUpdate remembers when the last recognized LED command arrived.

<a id="arduino-startup"></a>

#### 3. Arduino: setup runs once at startup

[Original LandlordController.ino, lines 109–122](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/LandlordController/LandlordController.ino#L109-L122)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">void setup() {
  Serial.begin(9600);
  pinMode(PIN_JOY_SW, INPUT_PULLUP);
  pinMode(PIN_PLAY_BTN, INPUT_PULLUP);
  pinMode(LED_P0, OUTPUT);
  pinMode(LED_P1, OUTPUT);
  pinMode(LED_P2, OUTPUT);
  digitalWrite(LED_P0, HIGH);
  digitalWrite(LED_P1, HIGH);
  digitalWrite(LED_P2, HIGH);
  delay(500);
  setTurnLEDs(-1);
  Serial.println(&quot;READY&quot;);
}</pre>
{% endraw %}

setup opens serial communication at 9600 baud and configures the two button pins as INPUT_PULLUP. With this setting, an unpressed button reads HIGH; pressing it connects the input to GND and makes it read LOW. The joystick click and the separate D9 button are separate switches, even though the program reads them in the same way.

The three LED pins become outputs. All three lights turn on for half a second as a startup check, then turn off. Finally, the board sends READY to the computer. When the connected browser receives READY, it calls syncTurnLED to restore the current turn indication. The startup flash is not a game turn: during normal play, the browser tells the board which single turn LED to light.

<a id="arduino-joystick"></a>

#### 4. Arduino: converting a tilt into one navigation message

[Original LandlordController.ino, lines 67–77](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/LandlordController/LandlordController.ino#L67-L77)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">void readJoystickInputs() {
  int x = analogRead(PIN_JOY_X);
  if (x &gt;= CENTER_MIN &amp;&amp; x &lt;= CENTER_MAX) movedX = false;
  if (!movedX &amp;&amp; (x &lt; THRESHOLD_LOW || x &gt; THRESHOLD_HIGH)) {
    bool left = (x &lt; THRESHOLD_LOW);
    if (REVERSE_X) left = !left;
    Serial.println(left ? &quot;NAV_LEFT&quot; : &quot;NAV_RIGHT&quot;);
    movedX = true;
  }
  // VRy is intentionally unused. Selection comes ONLY from the SW click.
}</pre>
{% endraw %}

analogRead reads the joystick’s horizontal position from A4. Returning to the center range resets movedX to false. A strong tilt is accepted only while movedX is false. The code determines left or right, applies the orientation reversal, sends NAV_LEFT or NAV_RIGHT, and sets movedX to true.

Holding the joystick to one side therefore sends one navigation command rather than continuously scrolling. To move another card, I return it to center and tilt again. The vertical axis is intentionally unused. Tilting browses cards; pressing the joystick straight down is what selects or deselects a card.

<a id="arduino-buttons"></a>

#### 5. Arduino: accepting one stable button press

[Original LandlordController.ino, lines 54–65](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/LandlordController/LandlordController.ino#L54-L65)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">void readButton(ButtonState &amp;button, const char *command) {
  bool reading = digitalRead(button.pin);
  if (reading != button.lastReading) {
    button.changedAt = millis();
    button.lastReading = reading;
  }
  if (millis() - button.changedAt &gt;= DEBOUNCE_DELAY &amp;&amp;
      reading != button.stableState) {
    button.stableState = reading;
    if (reading == LOW) Serial.println(command);
  }
}</pre>
{% endraw %}

readButton works for either physical button. Whenever the raw reading changes, it records the time. Only after that reading has stayed unchanged for at least 50 milliseconds does it accept a new stable state. This filters the brief on/off flicker that a mechanical switch can produce when pressed or released.

The command is sent only when the newly accepted state is LOW, meaning pressed. Holding the button does not send more commands, and releasing it does not send a command. The command parameter lets the same function send TOGGLE_SELECT for the joystick click and BTN_PLAY for the separate mini button.

<a id="arduino-leds"></a>

#### 6. Arduino: choosing the correct turn LED

[Original LandlordController.ino, lines 48–52](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/LandlordController/LandlordController.ino#L48-L52)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">void setTurnLEDs(int player) {
  digitalWrite(LED_P0, player == 0 ? HIGH : LOW);
  digitalWrite(LED_P1, player == 1 ? HIGH : LOW);
  digitalWrite(LED_P2, player == 2 ? HIGH : LOW);
}</pre>
{% endraw %}

[Original LandlordController.ino, lines 79–88](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/LandlordController/LandlordController.ino#L79-L88)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">void handleIncomingLine() {
  int player;
  if (strcmp(incoming, &quot;TURN:0&quot;) == 0) player = 0;
  else if (strcmp(incoming, &quot;TURN:1&quot;) == 0) player = 1;
  else if (strcmp(incoming, &quot;TURN:2&quot;) == 0) player = 2;
  else if (strcmp(incoming, &quot;TURN:OFF&quot;) == 0) player = -1;
  else return;
  setTurnLEDs(player);
  lastTurnUpdate = millis();
}</pre>
{% endraw %}

setTurnLEDs compares the requested player with 0, 1, and 2, setting the matching LED HIGH and the others LOW. Player 0 is me on D4, player 1 is Bot 1 on D5, and player 2 is Bot 2 on D6. Passing -1 makes all three comparisons false, so all lights turn off.

handleIncomingLine accepts TURN:0, TURN:1, TURN:2, and TURN:OFF from the browser. strcmp returns zero when two strings match exactly. An unknown message is ignored. A recognized message changes the lights and updates lastTurnUpdate. These LEDs indicate whose turn it is, not whether my cards are valid or whether I won.

<a id="arduino-receive"></a>

#### 7. Arduino: collecting a complete message from the computer

[Original LandlordController.ino, lines 90–107](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/LandlordController/LandlordController.ino#L90-L107)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">void readIncomingSerial() {
  // Consume complete lines without blocking joystick/button polling.
  while (Serial.available() &gt; 0) {
    char c = Serial.read();
    if (c == &#x27;\r&#x27;) continue;
    if (c == &#x27;\n&#x27;) {
      if (!discardLine) {
        incoming[incomingLength] = &#x27;\0&#x27;;
        handleIncomingLine();
      }
      incomingLength = 0;
      discardLine = false;
    } else if (!discardLine) {
      if (incomingLength &lt; sizeof(incoming) - 1) incoming[incomingLength++] = c;
      else discardLine = true;
    }
  }
}</pre>
{% endraw %}

readIncomingSerial reads only bytes already available, so it can return to checking the controls without waiting for a whole message. It ignores the carriage return character, shown as \r, and treats a newline, shown as \n, as the end of a command. It then adds the string-ending zero character and calls handleIncomingLine.

A command may arrive over several passes through loop; incoming preserves the unfinished text. If the line is too long for the buffer, the code discards it until the next newline. This prevents a partial or oversized message from being treated as a valid turn command.

<a id="arduino-loop"></a>

#### 8. Arduino: repeating input checks and clearing stale lights

[Original LandlordController.ino, lines 124–131](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/LandlordController/LandlordController.ino#L124-L131)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">void loop() {
  readJoystickInputs();
  readButton(selectButton, &quot;TOGGLE_SELECT&quot;);
  readButton(playButton, &quot;BTN_PLAY&quot;);
  readIncomingSerial();
  // Turn lights off if the game stops sending its one-second heartbeat.
  if (millis() - lastTurnUpdate &gt; 3500) setTurnLEDs(-1);
}</pre>
{% endraw %}

After setup, loop repeats continuously. It checks joystick movement, reads the joystick click as TOGGLE_SELECT, reads the D9 mini button as BTN_PLAY, and collects messages coming back from the computer. The Arduino reports physical actions; it leaves card rules and turn decisions to the browser.

The final condition turns all LEDs off if more than 3.5 seconds have passed since a recognized turn message. The browser normally repeats the current turn every second. If communication stops, an old turn light therefore does not stay on indefinitely. This timeout does not end or restart the card game.

### Browser game

<a id="browser-visible-controls"></a>

#### 9. Browser controls and the hand area — HTML excerpts

[Original game.html, lines 300–309](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L300-L309)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">                    &lt;button id=&quot;btn-hint&quot; class=&quot;bg-slate-800 hover:bg-slate-700 text-amber-300 border border-slate-600 px-3 py-1.5 rounded-xl text-xs font-bold transition flex items-center gap-1 shadow&quot;&gt;
                        &lt;i class=&quot;fa-solid fa-lightbulb&quot;&gt;&lt;/i&gt;
                        &lt;span&gt;提示 Hint (Tíshì)&lt;/span&gt;
                    &lt;/button&gt;
                    &lt;button id=&quot;btn-pass&quot; class=&quot;bg-slate-800 hover:bg-slate-700 text-slate-300 border border-slate-600 px-3.5 py-1.5 rounded-xl text-xs font-bold transition shadow&quot;&gt;
                        不要 Pass (Guò)
                    &lt;/button&gt;
                    &lt;button id=&quot;btn-play&quot; class=&quot;bg-gradient-to-r from-amber-400 to-amber-500 hover:from-amber-300 hover:to-amber-400 text-slate-950 px-5 py-1.5 rounded-xl text-xs font-extrabold shadow-lg transition transform active:scale-95&quot;&gt;
                        出牌 Play (Chū Pái)
                    &lt;/button&gt;</pre>
{% endraw %}

[Original game.html, lines 317–318](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L317-L318)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">            &lt;p class=&quot;text-xs text-sky-300 mb-2&quot;&gt;Joystick ←/→: highlight · Click joystick: select/deselect · Mini button: play / pass&lt;/p&gt;
            &lt;div id=&quot;human-hand-container&quot; class=&quot;min-h-[150px]&quot;&gt;&lt;/div&gt;</pre>
{% endraw %}

The HTML creates the visible Hint, Pass, and Play buttons. Each id gives JavaScript a specific element to find, while the class names control its appearance. These tags define the buttons; the click handlers later in the file give them their behavior. Hint selects a legal suggestion, Pass skips a response when passing is allowed, and Play submits the selected cards.

The instruction above human-hand-container explains the physical controls. JavaScript fills this initially empty container with the player's cards. Moving the joystick browses those cards, pressing the joystick changes selection, and the mini button performs the game action.

<a id="browser-focus-selection"></a>

#### 10. Showing focus and selection — CSS excerpts

[Original game.html, lines 39–49](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L39-L49)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">        .card-item.selected {
            transform: translateY(-30px) !important;
            box-shadow: 0 14px 24px -4px rgba(245, 158, 11, 0.6), 0 4px 8px rgba(0, 0, 0, 0.4);
            border-color: #f59e0b !important;
        }

        /* Joystick Cursor Focus Ring */
        .card-item.focused {
            outline: 3px solid #38bdf8;
            outline-offset: 3px;
        }</pre>
{% endraw %}

[Original game.html, lines 69–71](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L69-L71)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">        #human-hand-container &gt; .card-item.selected {
            transform: translateY(-10px) !important;
        }</pre>
{% endraw %}

The selected class marks cards chosen for the next play with an amber border, shadow, and upward movement. The focused class draws a blue outline around the card currently reached by the joystick. A card can have both classes: focus identifies the cursor position, while selection records which cards will be submitted.

The later rule specifically targets cards inside human-hand-container. It overrides the general selected movement, so cards in the actual hand rise by 10 pixels instead of 30. This keeps the selection visible without moving cards as far out of their row.

<a id="browser-state-turns"></a>

#### 11. Remembering the game and advancing turns — class excerpts

[Original game.html, lines 464–474](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L464-L474)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">                this.players=[
                    {id:0,name:&#x27;You&#x27;,isAI:false,hand:[],role:&#x27;PEASANT&#x27;,bids:-1,playsCount:0},
                    {id:1,name:&#x27;Bot 1&#x27;,isAI:true,hand:[],role:&#x27;PEASANT&#x27;,bids:-1,playsCount:0},
                    {id:2,name:&#x27;Bot 2&#x27;,isAI:true,hand:[],role:&#x27;PEASANT&#x27;,bids:-1,playsCount:0}
                ];
                this.holeCards=[];this.selectedCardIds=new Set();this.aiTimer=null;
                this.focusedCardIndex=0;this.gamePhase=&#x27;BIDDING&#x27;;this.currentTurn=0;
                this.landlordId=-1;this.highestBid=0;this.highestBidder=-1;
                this.baseScore=1;this.multiplier=1;this.bombCount=0;
                this.lastTrick=null;this.passCount=0;this.playedCards=[];
                this.springType=null;this.roundScores=null;</pre>
{% endraw %}

[Original game.html, lines 548–555](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L548-L555)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">            nextPlayTurn() {
                if(this.gamePhase!==&#x27;PLAYING&#x27;)return;
                if(this.passCount&gt;=2){this.currentTurn=this.lastTrick.playerId;this.lastTrick=null;this.passCount=0;}
                sendLedCommandToArduino(this.currentTurn);renderAllUI();
                const player=this.players[this.currentTurn],lead=!this.lastTrick;
                updatePlayedCardsTitle(lead?`👑 ${player.name} · 自由出牌 Lead`:`${player.name} · 跟牌 Follow`);
                if(!player.isAI) {
                    showAIReasonTag(lead?&#x27;Select any legal combination to lead.&#x27;:&#x27;Beat the same pattern and card count, play a bomb/rocket, or pass.&#x27;,true);</pre>
{% endraw %}

DouDiZhuGame stores the game's changing information. Player 0 is the human, player 1 is the left bot, and player 2 is the right bot. Each player has a hand and bidding information. selectedCardIds is a Set of chosen card IDs; focusedCardIndex separately records the joystick position. gamePhase and currentTurn determine which actions are currently allowed.

nextPlayTurn handles the next stage of play. After two consecutive passes, the last successful player regains the lead and lastTrick is cleared. The function sends the current player number to the Arduino, redraws the screen, and displays whether the player is leading or following.

<a id="browser-rules-bots"></a>

#### 12. Checking combinations and choosing bot moves — function excerpts

[Original game.html, lines 638–649](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L638-L649)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">            const pattern = (type, primaryRank, wing=&#x27;none&#x27;, chainLength=1) =&gt; ({type,primaryRank,length:n,wing,chainLength});
            const run = ranks.every((r,i) =&gt; i===0 || r===ranks[i-1]+1) &amp;&amp; ranks.at(-1)&lt;=14;
            if (n===2 &amp;&amp; counts.has(16) &amp;&amp; counts.has(17)) return pattern(PATTERNS.ROCKET,17);
            if (ranks.length===1) {
                const types={1:PATTERNS.SINGLE,2:PATTERNS.PAIR,3:PATTERNS.TRIO,4:PATTERNS.BOMB};
                return pattern(types[n],ranks[0]);
            }
            const trio = ranks.find(r =&gt; counts.get(r)===3);
            if (n===4 &amp;&amp; trio) return pattern(PATTERNS.TRIO_SOLO,trio,&#x27;single&#x27;);
            if (n===5 &amp;&amp; trio &amp;&amp; ranks.length===2) return pattern(PATTERNS.TRIO_PAIR,trio,&#x27;pair&#x27;);
            if (n&gt;=5 &amp;&amp; ranks.length===n &amp;&amp; run) return pattern(PATTERNS.STRAIGHT,ranks.at(-1),&#x27;none&#x27;,ranks.length);
            if (n&gt;=6 &amp;&amp; ranks.every(r=&gt;counts.get(r)===2) &amp;&amp; run) return pattern(PATTERNS.CHAIN_PAIRS,ranks.at(-1),&#x27;none&#x27;,ranks.length);</pre>
{% endraw %}

[Original game.html, lines 685–690](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L685-L690)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">            if (target.type===PATTERNS.INVALID || target.type===PATTERNS.ROCKET) return false;
            if (candidate.type===PATTERNS.ROCKET) return true;
            if (candidate.type===PATTERNS.BOMB &amp;&amp; target.type!==PATTERNS.BOMB) return true;
            return candidate.type===target.type &amp;&amp; candidate.length===target.length &amp;&amp;
                candidate.wing===target.wing &amp;&amp; candidate.chainLength===target.chainLength &amp;&amp;
                candidate.primaryRank&gt;target.primaryRank;</pre>
{% endraw %}

[Original game.html, lines 751–765](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L751-L765)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">        function chooseLeadPlay(hand) {
            const plays=generateLegalPlays(hand);
            const whole=plays.find(p=&gt;p.length===hand.length);
            if (whole) return whole;
            const ordinary=plays.filter(p=&gt;![PATTERNS.BOMB,PATTERNS.ROCKET].includes(evaluatePattern(p).type));
            return (ordinary.length?ordinary:plays).sort((a,b)=&gt;b.length-a.length || evaluatePattern(a).primaryRank-evaluatePattern(b).primaryRank)[0] || [];
        }

        function chooseFollowPlay(hand,targetTrick) {
            const plays=generateLegalPlays(hand,targetTrick);
            const weight=p=&gt;p.type===PATTERNS.ROCKET?2:p.type===PATTERNS.BOMB?1:0;
            return plays.sort((a,b)=&gt;{
                const pa=evaluatePattern(a),pb=evaluatePattern(b);
                return weight(pa)-weight(pb) || pa.primaryRank-pb.primaryRank;
            })[0] || null;</pre>
{% endraw %}

evaluatePattern describes a selection using its type, main rank, card count, and sequence information. This excerpt recognizes jokers, matching ranks, triples with attachments, straights, and consecutive pairs. Sequences stop at Ace, excluding 2s and jokers. canBeat compares legal combinations: a rocket wins over other patterns, a bomb beats ordinary patterns, and otherwise the shape and count must match with a higher main rank.

The game bots use local JavaScript rules, separate from generative ChatGPT. When leading, chooseLeadPlay prefers finishing the hand, then longer ordinary combinations and lower ranks. When following, chooseFollowPlay prefers an ordinary legal response before bombs or rockets, then a lower rank. The Hint button also uses these functions.

<a id="browser-hardware-actions"></a>

#### 13. Turning Arduino commands into game actions — handler excerpt

[Original game.html, lines 802–821](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L802-L821)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">            if (command === &#x27;NAV_LEFT&#x27; || command === &#x27;NAV_RIGHT&#x27;) {
                const delta = command === &#x27;NAV_LEFT&#x27; ? -1 : 1;
                game.focusedCardIndex = Math.max(0, Math.min(game.focusedCardIndex + delta, humanCards.length - 1));
                renderHumanHand();
                const focused = document.querySelector(&#x27;#human-hand-container .focused&#x27;);
                if (focused &amp;&amp; focused.scrollIntoView) focused.scrollIntoView({block:&#x27;nearest&#x27;, inline:&#x27;nearest&#x27;});
            } else if (command === &#x27;TOGGLE_SELECT&#x27; &amp;&amp; game.gamePhase === &#x27;PLAYING&#x27;) {
                const card = humanCards[game.focusedCardIndex];
                if (!card) return;
                if (game.selectedCardIds.has(card.id)) game.selectedCardIds.delete(card.id);
                else game.selectedCardIds.add(card.id);
                playSound(&#x27;card&#x27;);
                renderHumanHand();
            } else if (command === &#x27;BTN_PLAY&#x27;) {
                if (game.gamePhase === &#x27;BIDDING&#x27;) {
                    // Preserve button bidding, always using the lowest legal higher bid.
                    if (game.players[0].bids === -1 &amp;&amp; game.highestBid &lt; 3)
                        game.handleBid(0, game.highestBid + 1);
                } else playSelectedOrPass();
            }</pre>
{% endraw %}

handleHardwareInput receives command text from the serial reader. NAV_LEFT and NAV_RIGHT move focusedCardIndex within the hand's limits and redraw the cards; they do not add cards to the selection. TOGGLE_SELECT, sent by pressing the joystick, adds or removes the focused card's ID during play.

BTN_PLAY comes from the separate mini button. During bidding, it chooses the lowest legal bid above the current highest bid. During play, it calls playSelectedOrPass. Bidding Pass remains an onscreen choice. Before this excerpt, the handler blocks gameplay input when the rules window is open, the round is over, or it is another player's turn.

<a id="browser-play-pass"></a>

#### 14. Deciding whether to play or pass — complete function

[Original game.html, lines 773–793](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L773-L793)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">        function playSelectedOrPass() {
            if (game.gamePhase !== &#x27;PLAYING&#x27; || game.currentTurn !== 0) return;
            const cards = game.players[0].hand.filter(c =&gt; game.selectedCardIds.has(c.id));
            if (cards.length === 0) {
                if (!game.lastTrick || game.lastTrick.playerId === 0) {
                    showToastNotice(&#x27;You are leading: select cards to play. You cannot pass.&#x27;);
                } else {
                    game.passTurnAction(0);
                }
                return;
            }
            if (evaluatePattern(cards).type === PATTERNS.INVALID) {
                showToastNotice(&#x27;These cards are not a valid combination.&#x27;);
                return;
            }
            if (game.lastTrick &amp;&amp; game.lastTrick.playerId !== 0 &amp;&amp; !canBeat(cards, game.lastTrick)) {
                showToastNotice(&#x27;Select a stronger combination, or deselect all cards to pass.&#x27;);
                return;
            }
            game.playCardsAction(0, cards);
        }</pre>
{% endraw %}

playSelectedOrPass is shared by the onscreen Play button and the mini button during play. It first requires the human's turn, then collects the selected cards from the actual hand. With no cards selected, it passes only when responding to another player's trick. A player who is leading must select a legal combination instead.

With cards selected, the function checks that they form a valid pattern and, when following, beat the current trick. Invalid selections produce a message without playing the cards. Only a selection that passes these checks reaches playCardsAction, which updates the game state.

<a id="browser-render-hand"></a>

#### 15. Drawing cards and supporting mouse input — rendering excerpt

[Original game.html, lines 1003–1018](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L1003-L1018)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">            hand.forEach((card, idx) =&gt; {
                const isSelected = game.selectedCardIds.has(card.id);
                const isFocused = (idx === game.focusedCardIndex &amp;&amp; game.currentTurn === 0 &amp;&amp; game.gamePhase === &#x27;PLAYING&#x27;);
                const cardEl = createCardElement(card, idx + 1);

                if (isSelected) cardEl.classList.add(&#x27;selected&#x27;);
                if (isFocused) cardEl.classList.add(&#x27;focused&#x27;);

                cardEl.onclick = () =&gt; {
                    if (game.gamePhase !== &#x27;PLAYING&#x27; || game.currentTurn !== 0) return;
                    game.focusedCardIndex = idx;
                    playSound(&#x27;card&#x27;);
                    if (game.selectedCardIds.has(card.id)) game.selectedCardIds.delete(card.id);
                    else game.selectedCardIds.add(card.id);
                    renderHumanHand();
                };</pre>
{% endraw %}

renderHumanHand rebuilds the displayed hand from the cards stored in game.players[0].hand. For each card, it checks whether the card ID is selected and whether its position matches the joystick cursor. It then adds the corresponding CSS classes. The blue focus ring is shown only during the human's playing turn.

Each card also receives a mouse click handler. Clicking a card moves focus to it and toggles its selection, then redraws the hand. The turn check prevents clicks from selecting cards during bidding or another player's turn. Mouse input and hardware input therefore update the same selection state.

<a id="browser-button-wiring"></a>

#### 16. Connecting screen buttons to functions — event-handler excerpt

[Original game.html, lines 1072–1094](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L1072-L1094)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">        document.getElementById(&#x27;btn-hint&#x27;).onclick = () =&gt; {
            if(game.gamePhase!==&#x27;PLAYING&#x27;||game.currentTurn!==0)return;
            const hand=game.players[0].hand;
            const cards=game.lastTrick?chooseFollowPlay(hand,game.lastTrick):chooseLeadPlay(hand);
            game.selectedCardIds=new Set((cards||[]).map(c=&gt;c.id));
            renderHumanHand();
            showToastNotice(cards&amp;&amp;cards.length?&#x27;Legal suggestion selected. Press Play to use it.&#x27;:&#x27;No legal response. Press the mini button to pass.&#x27;);
        };
        document.getElementById(&#x27;btn-play&#x27;).onclick = playSelectedOrPass;
        document.getElementById(&#x27;btn-pass&#x27;).onclick = () =&gt; {
            if (game.gamePhase !== &#x27;PLAYING&#x27; || game.currentTurn !== 0 ||
                !game.lastTrick || game.lastTrick.playerId === 0) return;
            game.passTurnAction(0);
        };
        document.getElementById(&#x27;btn-deselect&#x27;).onclick = () =&gt; { game.selectedCardIds.clear(); renderHumanHand(); };
        document.getElementById(&#x27;btn-restart&#x27;).onclick = () =&gt; { game.init(); };
        document.getElementById(&#x27;btn-rules&#x27;).onclick = () =&gt; { document.getElementById(&#x27;rules-modal&#x27;).classList.remove(&#x27;hidden&#x27;); };
        document.getElementById(&#x27;btn-close-rules&#x27;).onclick = () =&gt; { document.getElementById(&#x27;rules-modal&#x27;).classList.add(&#x27;hidden&#x27;); };
        document.getElementById(&#x27;btn-close-rules-bottom&#x27;).onclick = () =&gt; { document.getElementById(&#x27;rules-modal&#x27;).classList.add(&#x27;hidden&#x27;); };
        document.getElementById(&#x27;btn-audio-toggle&#x27;).onclick = () =&gt; {
            audioEnabled = !audioEnabled;
            document.getElementById(&#x27;audio-icon&#x27;).className = audioEnabled ? &#x27;fa-solid fa-volume-high&#x27; : &#x27;fa-solid fa-volume-xmark&#x27;;
        };</pre>
{% endraw %}

These onclick assignments connect the HTML buttons to JavaScript actions. Hint finds a legal lead or response, selects those cards, and redraws the hand; the player still presses Play to use the suggestion. Play calls the shared playSelectedOrPass function. The separate Pass button checks that another player's trick exists before passing.

Deselect All clears the selected card IDs, and New Game calls game.init to restart. Rules opens the rules window, while its close buttons hide it again. The audio button toggles audioEnabled and changes the speaker icon. These controls provide browser access to the game alongside the Arduino controls.

### USB serial connection

<a id="serial-connect"></a>

#### 17. Connection: asking permission and opening the USB serial port

[Original game.html, lines 891–909](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L891-L909)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">        async function connectSerial() {
            if(serialBusy||serialClosing)return;
            if(serialPort){await disconnectSerial();return;}
            if(!(&#x27;serial&#x27; in navigator)){setSerialStatus(&#x27;Open this downloaded HTML in desktop Chrome or Edge to connect Arduino.&#x27;);return;}
            serialBusy=true;$(&#x27;connect-btn&#x27;).disabled=true;
            try {
                const port=await navigator.serial.requestPort();
                await port.open({baudRate:9600});
                serialPort=port;serialWriter=port.writable.getWriter();
                serialBuffer=&#x27;&#x27;;serialDropping=false;
                serialReadTask=readSerialLoop(port);
                $(&#x27;connect-btn&#x27;).textContent=&#x27;Disconnect Arduino&#x27;;
                setSerialStatus(&#x27;Connected · 9600 baud&#x27;);
                await syncTurnLED();
            } catch(error) {
                if(serialPort)await disconnectSerial();
                setSerialStatus(error.name===&#x27;NotFoundError&#x27;?&#x27;No port selected.&#x27;:&#x27;Connection failed: &#x27;+error.message+&#x27; Close Arduino Serial Monitor and try again.&#x27;);
            } finally {serialBusy=false;$(&#x27;connect-btn&#x27;).disabled=false;}
        }</pre>
{% endraw %}

connectSerial runs when Connect Arduino is clicked. It prevents overlapping connection attempts, checks whether the browser supports Web Serial, and asks the user to choose a port. It opens that port at 9600 baud, matching Serial.begin(9600) on the Arduino. If a connection already exists, the same button disconnects it.

The browser obtains a writer for outgoing bytes and starts readSerialLoop for incoming bytes. It changes the button to Disconnect Arduino, displays the connected status, and sends the current turn to the LEDs. The catch block distinguishes canceling the port chooser from a failed connection and suggests closing Arduino Serial Monitor if the port cannot be opened.

<a id="serial-read"></a>

#### 18. Connection: receiving bytes and assembling command lines

[Original game.html, lines 829–843](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L829-L843)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">        let serialPort=null, serialReader=null, serialWriter=null, serialReadTask=null;
        let serialBusy=false, serialClosing=false, serialBuffer=&#x27;&#x27;, serialDropping=false;
        let writeQueue=Promise.resolve();
        function feedSerialText(text) {
            for(const ch of text) {
                if(ch===&#x27;\r&#x27;)continue;
                if(ch===&#x27;\n&#x27;) {
                    if(!serialDropping)handleHardwareInput(serialBuffer.trim());
                    serialBuffer=&#x27;&#x27;;serialDropping=false;
                } else if(!serialDropping) {
                    if(serialBuffer.length&lt;100)serialBuffer+=ch;
                    else {serialBuffer=&#x27;&#x27;;serialDropping=true;}
                }
            }
        }</pre>
{% endraw %}

[Original game.html, lines 856–872](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L856-L872)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">        async function readSerialLoop(port) {
            const decoder=new TextDecoder();
            try {
                serialReader=port.readable.getReader();
                while(!serialClosing) {
                    const {value,done}=await serialReader.read();
                    if(done)break;
                    feedSerialText(decoder.decode(value,{stream:true}));
                }
            } catch(error) {
                if(!serialClosing)setSerialStatus(&#x27;Arduino disconnected: &#x27;+error.message);
            } finally {
                if(serialReader){serialReader.releaseLock();serialReader=null;}
                // Let this read task finish before disconnect waits for it.
                if(!serialClosing)Promise.resolve().then(()=&gt;disconnectSerial());
            }
        }</pre>
{% endraw %}

The first variables remember the connection, reader, writer, unfinished command, and whether the connection is closing. readSerialLoop waits asynchronously for incoming byte chunks. TextDecoder converts those bytes into text, and feedSerialText collects characters until a newline arrives. The browser can keep responding while it waits for serial input.

A read might contain only TOGGLE_ and the next read SELECT followed by a newline. The buffer joins them before calling handleHardwareInput once with TOGGLE_SELECT. Carriage returns are ignored, and lines longer than 100 characters are discarded. If reading ends or fails, the code releases the reader and begins disconnect cleanup.

<a id="serial-write"></a>

#### 19. Connection: sending the current turn back to the board

[Original game.html, lines 845–855](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L845-L855)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">        function sendSerialLine(line) {
            const writer=serialWriter;
            if(!writer)return Promise.resolve();
            writeQueue=writeQueue.then(()=&gt;writer.write(new TextEncoder().encode(line+&#x27;\n&#x27;)))
                .catch(()=&gt;{if(!serialClosing)setSerialStatus(&#x27;Connection lost. Disconnect and reconnect Arduino.&#x27;);});
            return writeQueue;
        }
        function syncTurnLED() {
            const turn=[&#x27;OVER&#x27;,&#x27;REDEAL&#x27;].includes(game.gamePhase)?&#x27;OFF&#x27;:game.currentTurn;
            return sendSerialLine(&#x27;TURN:&#x27;+turn);
        }</pre>
{% endraw %}

[Original game.html, lines 910–913](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L910-L913)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">        function sendLedCommandToArduino(turnVal) {
            return sendSerialLine(&#x27;TURN:&#x27; + turnVal);
        }
        setInterval(() =&gt; { if (!serialClosing) syncTurnLED(); }, 1000);</pre>
{% endraw %}

sendSerialLine adds a newline, converts the text into bytes with TextEncoder, and writes it to the serial port. writeQueue chains writes in order so separate parts of the game can send messages without overlapping their writes. With no connected writer, the function simply returns.

syncTurnLED sends TURN followed by the current player number, or TURN:OFF when the phase is OVER or REDEAL. sendLedCommandToArduino is another route used when turns change. The timer repeats synchronization every 1000 milliseconds. This regular update works with the Arduino’s 3500-millisecond timeout to clear a stale light after communication stops.

<a id="serial-disconnect"></a>

#### 20. Connection: disconnecting and releasing the port

[Original game.html, lines 873–890](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/f49cb4a02af406a114b172683f17b0eb5e30dbd9/assets/fight-the-landlord/game.html#L873-L890)

{% raw %}
<pre class="journal-source" style="font:inherit; white-space:pre-wrap; overflow-wrap:anywhere; background:transparent; border:0; border-radius:0; padding:0; color:inherit;">        async function disconnectSerial() {
            if(serialClosing)return;
            serialClosing=true;
            try {
                await sendSerialLine(&#x27;TURN:OFF&#x27;);
                if(serialReader)await serialReader.cancel().catch(()=&gt;{});
                if(serialReadTask)await serialReadTask;
                await writeQueue;
                if(serialWriter){serialWriter.releaseLock();serialWriter=null;}
                if(serialPort)await serialPort.close().catch(()=&gt;{});
            } finally {
                serialPort=null;serialReadTask=null;serialBuffer=&#x27;&#x27;;serialDropping=false;
                serialClosing=false;serialBusy=false;
                $(&#x27;connect-btn&#x27;).textContent=&#x27;Connect Arduino&#x27;;
                $(&#x27;connect-btn&#x27;).disabled=false;
                setSerialStatus(&#x27;Disconnected&#x27;);
            }
        }</pre>
{% endraw %}

disconnectSerial first marks the connection as closing and attempts to send TURN:OFF. It cancels the reader, waits for the read task and queued writes, releases the writer, and closes the port. Releasing readers and writers matters because they hold access to the streams while in use.

The finally block clears the saved connection and partial-message state, re-enables the button, and changes the display to Disconnected. If communication stops while the Arduino remains powered, its own timeout is the backup for clearing the LEDs. Disconnecting its only power source turns the LEDs off immediately. Reconnecting starts with a fresh message buffer.

## Execution and troubleshooting

One concrete correction was matching the shared files to my hardware: the earlier sketch listed pin 3 for the separate button, and I confirmed that my button is on pin 9. The shared files now use D9. A physical retest of that updated sketch is a next check to record.

If an input stopped working, I would check the wiring and pin first, then the Arduino message, the USB connection, and the game response. This would help locate the problem before changing unrelated code. A new test log would record the expected result, what happened, the change made, and the result after trying again.

## My reflection

I think the results are quite good because the physical controller worked. Having a working prototype gives me something concrete to show for the project. Next time, I would record a few checkpoints alongside my deadline so I could show more clearly how I used my class time and what I completed at each stage.

## Photos and demonstration


![Arduino Uno, breadboard, joystick, separate push button, and turn LEDs](/assets/fight-the-landlord/controller-wiring.jpg)

*My controller hardware. The separate play button uses digital pin 9; the joystick click uses pin 2.*

![Controller connected beside a laptop displaying Fight the Landlord](/assets/fight-the-landlord/controller-setup.jpg)

*The controller connected beside the laptop with Fight the Landlord on screen.*

<video controls playsinline preload="metadata" poster="/assets/fight-the-landlord/controller-setup.jpg" style="display:block; width:100%; height:auto; border-radius:12px;">
  <source src="/assets/fight-the-landlord/controller-demo.mp4" type="video/mp4">
  Your browser does not support embedded video. <a href="/assets/fight-the-landlord/controller-demo.mp4">Open the project demonstration</a>.
</video>

*[Watch the project demonstration](/assets/fight-the-landlord/controller-demo.mp4) · About 74 seconds.*

## Try the prototype


The game lets me play against two programmed opponents. I can use the mouse, or connect the Arduino controller to move between cards, select cards, and play them. Three LEDs show whose turn it is.

For the physical controller, [open the game in a full window](/assets/fight-the-landlord/game.html) in desktop Chrome or Edge. Upload the Arduino sketch, close Serial Monitor, and click Connect Arduino. The game and the Arduino both use 9600 baud. The project ZIP includes the wiring and setup instructions.

<iframe
  src="/assets/fight-the-landlord/game.html"
  title="Play Fight the Landlord"
  width="100%"
  height="720"
  style="display:block; width:100%; height:720px; border:0; border-radius:12px; background:#0d2818;"
  loading="lazy"
  allow="serial; autoplay; fullscreen"
  allowfullscreen>
</iframe>

*The embedded game can be played with the mouse. Use the full-window link above when connecting the Arduino.*

[View the browser game source](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/fight-the-landlord/game.html) · [View the Arduino sketch](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/fight-the-landlord/LandlordController/LandlordController.ino)
