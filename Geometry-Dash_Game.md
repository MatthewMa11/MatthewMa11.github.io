---
layout: default
title: Geometry Dash My Version
permalink: /geometry-dash-game/
---

[← Back to my homepage](/)

# Geometry Dash My Version

**Innovators Journal · October 13, 2026 · Updated October 15, 2026**

[Play the five-level game](/assets/neon-dash/neon-dash-v4-doublejump.html) · [Download the Word journal](/assets/neon-dash/neon-dash-development-journal.docx) · [View the source](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/neon-dash/neon-dash-v4-doublejump.html)

In class, I used WebGPT to make a game like Geometry Dash. I chose the features and gave feedback while WebGPT generated the code. The linked game and screenshots show the five-level version. Later versions from my WebGPT chat add more features.

## My development process

### October 12

I started with jumping, spikes, safe platforms, levels, and portals. The first game showed a black screen, so I reported it and WebGPT added canvas size checks. Level 1 was too hard, so I asked for an easier start and harder later levels. The base speed dropped from 7.2 to 6.0. I then requested a double jump and comments to help explain the code. Landing restores the extra jump.

### October 14

I asked for menus, Dash-Lite and Dash-Extreme with 50 levels each, English, Spanish, Chinese, and a tutorial. WebGPT returned an expanded version with generated levels and saved progress. Another revision added wave movement: a blue portal changes the cube into an arrow, holding makes it rise, releasing makes it fall, and a yellow portal finishes the level. My last request was to make Extreme much harder; the chat ends before a reply. These later versions still need testing.

## What I learned

HTML provides the canvas and buttons, CSS controls their appearance, and JavaScript handles movement and rules. Variables remember things like the extra jump, while hitboxes check collisions. A square is safe to land on from above, but hitting its side makes the player lose. The expanded version also introduced repeatable level generation, translated text, and saved settings.

Next time, I would test one short level before adding more features. I would describe what I expected, what happened, and how to repeat a problem. Saving versions, screenshots, and test results would help me track changes and understand the code better.

![Game start screen with jump controls](/assets/neon-dash/menu.png)

*The five-level game's start screen. Some symbols still display as strange text.*

## How the published game works

### The canvas

`<canvas id="game"></canvas>` is at line 58. `syncSize()` matches it to the browser window (lines 78–89). These screenshots use 1280 × 600 pixels. A wider window shows more of the map; a taller one moves the ground. Blocks stay the same size.

### The state variables

Most variables are declared under STATE; jump controls are under INPUT.

| Variable | What it remembers | Code lines |
| --- | --- | --- |
| `state`, `levelIdx`, `attempts` | Game status, level, and tries | 198–200 |
| `player` | Position `x, y`, vertical speed `vy`, rotation, landing, and extra jumps | 212 |
| `W`, `H`, `camX` | Canvas size and camera position | 79; 200 |
| `jumpHeld`, `jumpQueued` | Holding jump or making a new press | 230 |

Forward speed uses `BASE_SPEED` and the level's speed setting (line 207).

### The game loop

`loop(now)` repeats through `requestAnimationFrame`, often about 60 times a second (lines 561–574). `update(dt)` moves objects and checks the rules. `draw()` draws the next picture. `dt` adjusts for the time between frames.

```javascript
if(state !== 'menu' && state !== 'win') update(dt);
try { draw(); } catch(e) {}
requestAnimationFrame(loop);
```

### Input handling

Space, Up, W, clicking, or tapping makes the player jump. The listener changes `jumpHeld` and `jumpQueued` (lines 230–243). `update()` changes `player.vy` to move upward. Releasing and pressing again in the air uses the double jump; landing restores it.

### Collision and win or lose logic

`rectsOverlap(a,b)` checks whether invisible rectangular hitboxes overlap (line 258):

```javascript
a.x < b.x+b.w && a.x+a.w > b.x &&
a.y < b.y+b.h && a.y+a.h > b.y
```

A spike collision runs `die()`. Blocks are safe when falling onto their tops; the check is `player.vy >= 0 && prevBottom <= oy + 14` (lines 364–371). A crash restarts the level. Reaching the portal starts the next level, and completing all five shows YOU WIN! (lines 308–315 and 383).

### Scoring and progress

There are no points. `attempts++` adds a try after a crash (line 262). Lines 380–381 calculate `player.x / levelEndX()` and update the progress bar. A recorded test reached Level 2 with a new background, then confirmed that a spike added an attempt.

*Line numbers refer to the linked five-level source file.*

## Screenshots and goals

![The square double jumps above a spike](/assets/neon-dash/double-jump.png)

*The ring below the square shows that the extra jump was used.*

![The portal near the end of Level 1](/assets/neon-dash/level-one-portal.png)

![Level 2 with a different background](/assets/neon-dash/level-two.png)

*The Level 1 portal and the new background in Level 2.*

### Goals for October 15 to October 18

- **By October 16:** Test jumping ten times, including a second jump, an attempted third jump, and landing to restore the extra jump.
- **By October 16:** Check that the expanded tutorial teaches jumping, double jumping, and wave controls. Test its first visit and replay options, and save a screenshot.
- **By October 18:** Try each published level five times. Sample early, middle, and late levels in both expanded modes. Record progress, unfair sections, and whether both portals work.
- **By October 18:** Compare the same level at 800 × 600 and 1280 × 720. Save screenshots and check the ground, controls, and visible map.
- **By October 18:** Check all three languages for missing text or layout problems. Reload the expanded game to check that language and unlocked levels are saved.

## Complete WebGPT prompts

### Original game

> Create a Geometry Dash-like game where you have to press space or other buttons to jump, and there are squares, which are safe to land on, and spikes, which will end the game once you touch them. Be creative with the maps. Add levels to this thing. once you completeed a level there will be a portal for you to enter. The color of the background color changes as you level up. Make everything very cool and intriguing.

### Menus levels and languages

> Create a starting menu, add options for Dash-Lite and Dash-Extreme, each with 50 levels, each increasing in difficulty, and a setting to adjust the language, supporting Spanish, English, and Chinese. When you first enter the game, you are required to go through an instruction course that teaches you which button to press and what it does, such as jumping, double jumping, etc. Make the UI 1000000000% better.

### Wave movement and portals

> I want that when you hit the portal, you switch into the flying line- another form, just like the real Geometry Dash- and when you hit a yellow, you win. Check how the real Geometry Dash is. I want it just like it is in the original game.

I used WebGPT for the game code and Codex to help organize and edit this journal.
