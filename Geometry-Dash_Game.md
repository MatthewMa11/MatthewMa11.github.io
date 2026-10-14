---
layout: default
title: Neon Dash Development Journal
permalink: /neon-dash-journal/
---

[← Back to my homepage](/)

# Neon Dash Development Journal

**Innovators Journal entry · October 13, 2026**

[Play Neon Dash](/assets/neon-dash/neon-dash-v4-doublejump.html) · [Download the Word journal](/assets/neon-dash/neon-dash-development-journal.docx) · [View the game source](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/neon-dash/neon-dash-v4-doublejump.html)

My class project was to use WebGPT to create a Geometry Dash style game. The result is NEON DASH, a browser game with five levels, automatic forward movement, jumps and a double jump. This entry documents my request, the current outcome, how the code works, and my goals for the rest of the week.

## My process

I gave WebGPT specific rules: use Space or other buttons to jump, make squares safe to land on, and make spikes end the attempt. I also asked for creative maps, multiple levels, a portal after each level, and background colors that change as the player levels up. My visual direction was to make everything “very cool and intriguing.”

The current file turns that request into one HTML document containing the interface, styling and game logic. To understand the result, I can follow a key press into the jump variables, then into movement and collision checks. Comparing those checks with my original wording also reveals details that need clearer instructions.

![Neon Dash start screen with keyboard controls and double jump instructions](/assets/neon-dash/menu.png)

*Figure 1. The current start screen explains jumping, spikes and portals. The literal \u26A1 and \u2014 text is a visible menu issue to fix in a future revision.*

## The current outcome

The five maps are Stereo Genesis, Azure Drift, Violet Voltage, Crimson Circuit and Emerald Singularity. Their difficulty labels range from EASY to INSANE. The build adds glowing obstacles, a rotating square player, trails and a double jump. A browser check reached the Level 1 portal and continued into Level 2 with a different background. A later spike collision increased ATTEMPTS from 1 to 2 and restarted the current level.

## How the game works

### The canvas

The drawing surface is `<canvas id="game"></canvas>` in the body at line 58. CSS at line 10 fills the browser window. It has no fixed width or height in the HTML: syncSize() reads its displayed size and assigns canvas.width, canvas.height, W and H at lines 78–89. The screenshots use a 1280 × 600 pixel canvas.

A wider window shows more of the level ahead. A taller one moves the ground because groundY() returns H × 0.78 (line 206). At 600 pixels tall, the ground is y = 468. Obstacle units stay at U = 46 and the player stays about 37.72 pixels wide. Changing the HTML width or height alone will be overwritten by syncSize(); a useful experiment must change the window or the canvas CSS size.

### The state variables

State variables remember what is happening between frames. They are declared in the script, mainly under STATE, with input flags under INPUT. Constants such as gravity and jump speed are separate settings at lines 91–98.

| Variable | What it stores | Source lines |
| --- | --- | --- |
| state | menu, play, dead, levelcomplete or win | 198; 261; 271; 313 |
| levelIdx and attempts | Current level and cumulative attempt count | 199 |
| player | x, y, vy, rot, onGround and airJumps | 212 |
| W, H and camX | Canvas dimensions and camera position | 79; 200 |
| jumpHeld and jumpQueued | Held input and a fresh jump request | 230 |
| deadTimer and warpTimer | Retry and level transition timing | 202 |
| particles, rings and trail | Animation effects rather than points | 200 |

### The game loop

loop(now), at lines 561–574, repeats through requestAnimationFrame(loop). The browser schedules it around screen refresh, often near 60 frames per second, rather than at a guaranteed fixed rate. Each frame updates the game through update(dt) and draws it through draw(). dt measures elapsed time in units of about 16.67 milliseconds and is capped at 2.5 to limit large movement steps.

```javascript
const dt = Math.min(2.5, (now - last) / 16.67);
if(state !== 'menu' && state !== 'win') update(dt);
try { draw(); } catch(e) {}
requestAnimationFrame(loop);
```

### Input handling

The keydown listener accepts Space, ArrowUp and W (lines 233–239). Mouse and touch events also jump. pressJump() sets jumpHeld and queues a fresh press. The next update sets player.vy to JUMP_V = −13.4 for a ground jump. A fresh airborne press sets vy to DJUMP_V = −12.2 and spends the one airJumps charge (lines 328–350). Releasing and pressing again is necessary for the double jump; holding a button automatically jumps after landing. Landing on the ground or a block refills the charge.

## Screenshots and game rules

![The player uses a double jump above a spike in Level 1](/assets/neon-dash/double-jump.png)

*Figure 2. A fresh second press activates the double jump. The ring below the player is its visual feedback.*

![The portal near the end of Level 1 Stereo Genesis](/assets/neon-dash/level-one-portal.png)

![Level 2 Azure Drift with a new background color](/assets/neon-dash/level-two.png)

*Figures 3 and 4. Left: the portal near the end of Stereo Genesis. Right: Azure Drift after the transition, with its new background and a fresh progress bar. These are screenshots from the current build.*

### Collision and win or lose logic

rectsOverlap(a,b), at line 258, checks whether two rectangles overlap on both axes. The actual comparison is:

```javascript
a.x < b.x+b.w && a.x+a.w > b.x &&
a.y < b.y+b.h && a.y+a.h > b.y
```

A spike uses a smaller rectangular hitbox inside its drawn triangle. If rectsOverlap(pb, sb) is true, die() runs (lines 357–364). A square is safe only when the player is falling onto its top: player.vy >= 0 && prevBottom <= oy + 14. Otherwise, touching the block ends the attempt (lines 367–371). Death sets state to dead; after deadTimer > 55, the same level restarts.

The portal finishes a level when player.x + PLAYER > portalX && player.x < portalX + U*1.5 (line 383). This is a horizontal position test. After the transition timer, the next level starts; completing the final level sets state to win and shows YOU WIN! (lines 308–315).

### Scoring and progress

There is no points score variable. attempts starts at 1 (line 199), and attempts++ updates the displayed count in die() (line 262). Progress is calculated with Math.min(1, player.x / levelEndX()) and written to progressBar.style.width as a percentage (lines 380–381). Attempts measure retries; the bar measures distance through the current map.

## What I understand and what comes next

### My reflection

My prompt defined the experience I wanted, but the code determines the exact rules. The clearest example is “squares are safe to land on.” The implementation allows landing from above, while a side collision still ends the attempt. That is a useful distinction to explain to a player and to check when giving WebGPT the next request.

I now understand how input, state and the loop connect. A key press changes flags, update() turns those flags into velocity, gravity changes the motion, and collision checks decide whether the player lands or dies. draw() displays the result. The double jump depends on remembering whether an extra jump is available, so the game needs more than a picture of the player.

The background change is also controlled by state. startLevel() sets bgHueTarget to the new map’s hue, and update() gradually moves bgHue toward that target (lines 217 and 292). The visual effect is part of the level transition, while the obstacle layouts and speed multipliers define the difficulty.

### Goals for October 14 to October 18

### Trace and test the double jump by Wednesday October 14

I will document the sequence from keydown to jumpQueued, player.vy and airJumps. I will run ten short trials covering a ground jump, a fresh second press, an attempted third jump, and a landing. Success means I can explain why only one extra jump is allowed before landing and show that the charge refills. This goal follows directly from the fresh press rule I found in the input code.

### Clarify the rules and repair menu text by Friday October 16

In a separate working copy, I will change the instructions to say that landing on top of squares is safe and side collisions are dangerous. I will replace the visible \u26A1 and \u2014 strings in the HTML with proper characters or HTML entities. Success means a new start screen screenshot has readable symbols and instructions that match the collision logic.

### Check map difficulty by Sunday October 18

I will record attempts and farthest progress for five runs of each map, using a practice copy to start harder levels directly if needed. I will identify one obstacle sequence where the timing feels unfair, adjust its spacing, and compare five more runs. Success means I have a before and after record and can explain the change using the map’s speed and jump range. This will help evaluate my request for creative maps rather than judging only their appearance.

### Test the canvas at two sizes by Sunday October 18

I will compare an 800 × 600 window with a 1280 × 720 window and save screenshots of the same level. I will check the controls, HUD and visible obstacles, and compare the ground positions of 468 and 561.6 pixels. Success means I can explain what resizing changes and identify any layout problem that affects play. This follows from the canvas sizing code rather than guessing that the whole game scales equally.

Code reference: [neon-dash-v4-doublejump.html](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/neon-dash/neon-dash-v4-doublejump.html). All source line numbers refer to the supplied 578-line version.
