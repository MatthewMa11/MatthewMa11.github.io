---
layout: default
title: Geometry Dash My Version
permalink: /neon-dash-journal/
---

[← Back to my homepage](/)

# Geometry Dash My Version

**Innovators Journal entry · October 13, 2026**

[Play Geometry Dash My Version](/assets/neon-dash/neon-dash-v4-doublejump.html) · [Download the Word journal](/assets/neon-dash/neon-dash-development-journal.docx) · [View the game source](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/neon-dash/neon-dash-v4-doublejump.html)

In class, I asked WebGPT to make a game like Geometry Dash. I wanted a square that jumps over spikes and goes through different levels. My game is called Geometry Dash My Version. It has five levels and a double jump.

## My process

I told WebGPT to use Space or other buttons to jump. I wanted squares that were safe to land on and spikes that would make the player lose. I also asked for creative maps, portals between levels, and background colors that changed with each level. I wanted the game to look cool and be fun to play.

WebGPT put the game in one HTML file. The file has the page layout, colors, and game code. Looking through it helped me understand what happens after a jump button is pressed.

![Game start screen with keyboard controls and double jump instructions](/assets/neon-dash/menu.png)

*Figure 1. The start screen shows the controls. Some symbols appear as strange text, like \u26A1. I want to fix that.*

## How it turned out

The square moves forward on its own, so the player has to time each jump. The game has glowing platforms, spikes, and a portal at the end of each level. A test showed that the Level 1 portal led to Level 2 and the background changed. Hitting a spike added one to the attempt count and restarted the level.

## What I learned

I learned that the game has several parts working together. Button presses start jumps, variables remember what is happening, and the game loop keeps everything moving. I also noticed that squares are only safe to land on from above. Hitting their sides still makes the player lose. I need to be more exact when I explain the rules to WebGPT.

## How the code works

### The canvas

The canvas is the area where the game is drawn. It is `<canvas id="game"></canvas>` at line 58. It fills the browser window. The screenshots use a canvas that is 1280 pixels wide and 600 pixels tall.

syncSize() matches the canvas to the window at lines 78–89. A wider window shows more of the map. A taller window moves the ground, which is 78 percent of the way down the screen. The blocks stay the same size.

### The state variables

Variables help the game remember things like the player’s position and how many tries I have used. Most are declared under STATE. The jump variables are under INPUT.

| Variable | What it remembers | Code lines |
| --- | --- | --- |
| state | menu, play, dead, levelcomplete or win | 198; 261; 271; 313 |
| levelIdx and attempts | The level and how many tries I have used | 199 |
| player | x, y, vy, rot, onGround and airJumps | 212 |
| W, H and camX | Screen size and where the camera is | 79; 200 |
| jumpHeld and jumpQueued | Whether I am holding or pressing jump | 230 |
| deadTimer and warpTimer | Waiting time before a retry or next level | 202 |
| particles, rings and trail | The glowing effects behind the player | 200 |

player remembers its position (x, y), up or down speed (vy), rotation (rot), whether it has landed (onGround), and extra jumps (airJumps). Speed uses BASE_SPEED and the level’s speed setting (line 207).

### The game loop

loop(now) repeats using requestAnimationFrame(loop) at lines 561–574, often about 60 times a second. update(dt) moves things and checks the rules. draw() draws the new picture. dt helps adjust for the time between frames.

```javascript
if(state !== 'menu' && state !== 'win') update(dt);
try { draw(); } catch(e) {}
requestAnimationFrame(loop);
```

### Input handling

Space, Up, W, clicking, or tapping makes the player jump. The keyboard listener is at lines 233–239. pressJump() changes jumpHeld and jumpQueued. update() changes player.vy to move the square upward. Letting go and pressing again in the air uses the double jump. Landing gives it back. Holding jump makes the square jump again after landing.

### Collision and win or lose logic

A hitbox is an invisible shape used to check whether things are touching. rectsOverlap(a,b) checks whether two rectangular hitboxes overlap (line 258):

```javascript
a.x < b.x+b.w && a.x+a.w > b.x &&
a.y < b.y+b.h && a.y+a.h > b.y
```

Touching a spike runs die() (line 364). The level restarts after a short wait (lines 303–305). A block is safe when falling onto its top. The check is player.vy >= 0 && prevBottom <= oy + 14 (line 368). Hitting its side makes the player lose.

Reaching the portal’s horizontal position runs completeLevel() (line 383). The next level starts after a short wait. Finishing all five levels shows YOU WIN! (lines 308–315).

### Scoring and progress

There is no points score. attempts starts at 1, and attempts++ adds one when the player crashes (line 262). The progress bar shows how far the square has gone through the level. Lines 380–381 divide player.x by levelEndX() and change the bar’s width.

Code reference: [neon-dash-v4-doublejump.html](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/neon-dash/neon-dash-v4-doublejump.html). All source line numbers refer to the supplied 578-line version.

## Screenshots and goals

![The player uses a double jump above a spike in Level 1](/assets/neon-dash/double-jump.png)

*Figure 2. The square is doing a double jump. The ring underneath it shows that the extra jump was used.*

![The portal near the end of Level 1 Stereo Genesis](/assets/neon-dash/level-one-portal.png)

![Level 2 Azure Drift with a new background color](/assets/neon-dash/level-two.png)

*Figure 3 shows the portal at the end of Level 1. Figure 4 shows the new background in Level 2.*

### My goals for October 14 to October 18

### Test the double jump by October 14

I will test jumping ten times. I will check the first jump, the double jump, and what happens if I try a third jump. I will also check that landing gives the extra jump back. This will help me understand the jump variables.

### Fix the instructions by October 16

I will create an introductory course at the beginning that tells the player which button to press and what each ability does. I will save a screenshot to show the changes.

### Check the levels by October 18

I will try each level five times and record my attempts and farthest progress. I can use a practice copy to start harder levels. If a section feels too hard, I will change the spacing and try it five more times to see if it feels better.

### Try two window sizes by October 18

I will try the game at 800 × 600 and 1280 × 720. I will take screenshots of the same level and compare the ground, controls, and how much of the map I can see. This will help me understand how the canvas size affects the game.
