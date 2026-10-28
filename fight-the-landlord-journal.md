---
layout: default
title: Fight the Landlord Controller
permalink: /fight-the-landlord-journal/
---

[← Back to my homepage](/)

# Fight the Landlord Controller

**Innovators Journal · October 27, 2026**

[Open the game](/assets/fight-the-landlord/game.html) · [Download the project ZIP](/assets/fight-the-landlord/landlord-controller-project.zip) · [Download the Word journal](/assets/fight-the-landlord/fight-the-landlord-controller-journal.docx) · [View the Arduino code](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/fight-the-landlord/LandlordController/LandlordController.ino)

I used ChatGPT to write the code for a Fight the Landlord card game and its Arduino controller. I also used ChatGPT to help me understand the code. The physical controller worked, and I think the results are quite good.

## Play my game

The game lets me play against two programmed opponents. I can use the mouse, or connect the Arduino controller to move between cards, select cards, and play them. Three LEDs show whose turn it is.

For the physical controller, [open the game in a full window](/assets/fight-the-landlord/game.html) in desktop Chrome or Edge. Upload the Arduino sketch, close Serial Monitor, and click **Connect Arduino**. The game and the Arduino both use **9600 baud**. The project ZIP includes the wiring and setup instructions.

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

## My development process and AI use

This project connects a physical controller to a browser game. The browser handles the cards, rules, and turns. The Arduino reads the joystick and buttons, then sends short commands to the browser through USB.

ChatGPT wrote the code and helped explain it to me. I used AI for both making the project and understanding what the different parts do. The explanations in this journal focus on how an input becomes a game action, how the program remembers a button press, and how information travels back to the LEDs.

My physical controller worked when I tried it. That is the result I can report from using the project. The original AI conversation is not included here, so the descriptions of my AI use are summaries rather than exact prompt quotations.

## How the controls work

| Control | Connection | What it does |
| --- | --- | --- |
| Joystick left and right | VRx → A4 | Moves the blue highlight to another card. Return to center before moving again. |
| Joystick click | SW → D2 | Selects or deselects the highlighted card. Selected cards rise and have a gold border. |
| Separate mini button | D3 → GND | Plays the selected cards. If no cards are selected, it passes when passing is allowed. |
| Three turn LEDs | D4, D5, D6 | Show the turns for me, Bot 1, and Bot 2. Each LED uses a 220–330 ohm series resistor. |

The joystick connects to 5 V and GND. Its up/down axis is unused; selecting a card means physically clicking the joystick down. The click switch and the separate play button have different digital inputs.

During bidding, the mini button chooses the next higher legal bid. Other bidding choices use the buttons on the screen. A player leading a trick cannot pass. Controller actions are ignored during the bots’ turns.

## Understanding my code

### Sending an action from the controller

These two lines in the Arduino loop read the buttons:

```cpp
readButton(selectButton, "TOGGLE_SELECT");
readButton(playButton, "BTN_PLAY");
```

Each name describes a different action. A joystick click sends `TOGGLE_SELECT`, while the mini button sends `BTN_PLAY`. The browser reads the message and decides what to do based on the current turn and phase of the game. This means the Arduino does not need to understand the card rules.

Both switches use `INPUT_PULLUP`. Their normal reading is HIGH, and pressing them connects the input to ground so it reads LOW. The program checks for that stable change before sending a command.

### Remembering one joystick movement

The joystick code includes this check:

```cpp
if (x >= CENTER_MIN && x <= CENTER_MAX) movedX = false;
```

The `movedX` variable remembers whether the current tilt already caused a move. After moving the highlight, the code sets it to true. Returning the joystick to the center resets it to false. This prevents a held tilt from racing through the cards.

The current center range is 400–620. Movement happens below 250 or above 770. `REVERSE_X = true` reverses left and right to match the intended joystick orientation.

### Sending information back to the LEDs

Communication works in both directions. The Arduino sends input commands, and the browser sends messages such as `TURN:0`, `TURN:1`, or `TURN:2` to choose a turn LED. `TURN:OFF` turns them all off.

The browser repeats the current turn every second. If the Arduino stops receiving a recognized turn message for more than 3.5 seconds, it turns the lights off. This prevents an old turn indicator from staying on after communication stops.

## Execution and troubleshooting

The physical controller worked. The checks below explain the safeguards in the final code and how to narrow down a problem if one occurs. They are not a claim that I personally encountered every failure listed.

### If the highlight moves too many times

I would first hold the joystick in one direction, then return it to center and tilt again. The expected result is one step for each tilt. The `movedX` flag handles this in the code. If it does not reset on the actual controller, checking the joystick’s center reading would help determine whether the center range needs adjustment.

### If one press causes several actions

A mechanical button can briefly switch between HIGH and LOW when pressed. The code waits **50 milliseconds** for a stable change. Each button keeps its own timing and state, so selecting and playing remain separate.

To check this, I would hold each button, release it, and press again. One stable press should produce one action. If a button does nothing, I would check its pin and ground connection, including whether the correct legs of the tactile switch were used.

### If the game receives no commands

I would check the connection before changing the game rules: the correct USB port, matching baud rates, and whether Serial Monitor is still using the port. The browser waits for a newline before processing a command, so text arriving in separate USB reads is assembled into one message.

This gives me a useful troubleshooting order: physical input, Arduino message, browser connection, then game response. Checking one stage at a time helps identify where the signal stops.

### If the play button seems to do nothing

The game may be rejecting an illegal move. It must be my turn, the selected cards must form a legal combination, and a leading player cannot pass. Invalid or weaker combinations stay selected so they can be adjusted. Checking the game’s notice and current turn helps distinguish a controller problem from a rule being enforced correctly.

The supplied README also reports software simulations for game rules, controller commands, and serial communication. Those are separate from my report that the physical controller worked; they do not establish that every possible hardware situation has been tested.

## My thinking and feelings

I think the results are quite good because the physical controller worked with the game. Having a working controller gives me a concrete result to show alongside the code.

I also wanted help understanding the code, which is why I used ChatGPT for explanations as well as code writing. For this project, the connection between a physical action and a message like `TOGGLE_SELECT` is a useful way to explain what the program does. The same idea applies to the LEDs: a change in the game becomes a message that changes an output on the Arduino.

## What I would improve next

A useful next step is to record specific tests instead of only saying that the controller works. I could check holding the joystick, repeated button presses, invalid card combinations, and unplugging and reconnecting the board. For each test, I would record what I expected, what happened, what I changed, and the result after trying again.

I would also add a photo of the physical controller and a short demonstration. These would make it easier to show the wiring, the controls in use, and how the LEDs follow the turns.

---

ChatGPT wrote the project code and helped me understand it. ChatGPT/Codex also helped organize and format this journal from my files and my description of the result.

[View the browser game source](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/fight-the-landlord/game.html) · [View the Arduino sketch](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/fight-the-landlord/LandlordController/LandlordController.ino)
