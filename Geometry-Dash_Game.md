---
layout: default
title: Geometry Dash My Version
permalink: /geometry-dash-game/
---

[← Back to my homepage](/)

# Geometry Dash My Version

**Innovators Journal · October 13, 2026 · Updated October 15, 2026**

[Play the five-level game](/assets/neon-dash/neon-dash-v4-doublejump.html) · [Download the Word journal](/assets/neon-dash/neon-dash-development-journal.docx) · [View the source](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/neon-dash/neon-dash-v4-doublejump.html)

In class, I used WebGPT to make a game like Geometry Dash. WebGPT wrote the HTML, CSS, and JavaScript. I gave the ideas, tried the results, and asked for fixes.

## My development process

### October 12: Starting and fixing the game

> Create a Geometry Dash-like game where you have to press space or other buttons to jump, and there are squares, which are safe to land on, and spikes, which will end the game once you touch them. Be creative with the maps. Add levels to this thing. once you completeed a level there will be a portal for you to enter. The color of the background color changes as you level up. Make everything very cool and intriguing.

The first result had a black screen. I reported it, and WebGPT added canvas size checks. Level 1 was too hard, so I asked for an easier start. WebGPT lowered the speed and spread out the spikes. I then asked for double jumping and code comments. The new version allowed one extra jump in the air, which resets after landing.

### October 14: Adding more choices

> Create a starting menu, add options for Dash-Lite and Dash-Extreme, each with 50 levels, each increasing in difficulty, and a setting to adjust the language, supporting Spanish, English, and Chinese. When you first enter the game, you are required to go through an instruction course that teaches you which button to press and what it does, such as jumping, double jumping, etc. Make the UI 1000000000% better.

After working on jumping, I asked for more ways to play. WebGPT returned a version with menus, generated levels, a tutorial, languages, and saved progress.

### October 14: Changing the portals

> I want that when you hit the portal, you switch into the flying line- another form, just like the real Geometry Dash- and when you hit a yellow, you win. Check how the real Geometry Dash is. I want it just like it is in the original game.

WebGPT added wave movement: a blue portal changes the player into an arrow, holding makes it rise, and releasing makes it fall. A yellow finish portal ends the level. My last request was to make Extreme ten times harder, but the chat ends before a reply.

The game and screenshots linked here show the five-level version. The later versions still need testing.

## What I learned

HTML sets up the page, CSS controls its look, and JavaScript runs the rules. Double jumping helped me understand how variables remember things: the game tracks the extra jump and resets it after landing. I also learned that invisible hitboxes decide collisions. A block's top is safe, but its sides can cause a crash.

## How I would build software next time

I would start with one level and test each AI change before adding more. I would read the changed code and save working versions. My prompts would explain what should happen, what went wrong, and how to repeat the problem.

## How the published game works

### The canvas

`<canvas id="game"></canvas>` is at line 58. `syncSize()` fits it to the window (lines 78–89). These screenshots are 1280 × 600 pixels. More width shows more map; more height moves the ground. Blocks stay the same size.

### The state variables

Under STATE (lines 198–212), `state`, `levelIdx`, and `attempts` track game status, level, and tries. `player` stores position, vertical speed, and jumps. Forward speed uses `BASE_SPEED` and the level setting. Jump flags are under INPUT (line 230).

### The game loop

`loop(now)` uses `requestAnimationFrame`, often about 60 times a second (lines 561–574). It calls `update(dt)` to move things and check rules, then `draw()` to draw the frame. `dt` adjusts for time between frames.

### Input handling

Space, Up, W, clicking, or tapping jumps. The listeners change `jumpHeld` and `jumpQueued`; `update()` changes `player.vy`. A new press in the air uses the extra jump (lines 230–243).

### Collision and win or lose logic

`rectsOverlap(a,b)` checks hitboxes (line 258):

```javascript
a.x < b.x+b.w && a.x+a.w > b.x &&
a.y < b.y+b.h && a.y+a.h > b.y
```

Spikes call `die()`. Safe landings check `player.vy >= 0 && prevBottom <= oy + 14` (lines 364–371). Crashes restart the level. Portals lead to the next level; finishing all five shows YOU WIN!

### Scoring and progress

There are no points. `attempts++` counts crashes (line 262). `player.x / levelEndX()` fills the progress bar (lines 380–381). A recorded test reached Level 2 and confirmed that a spike added an attempt.

*Line numbers refer to the linked five-level source file.*

## Screenshots and goals

![Game start screen with jump controls](/assets/neon-dash/menu.png)

![The square double jumps above a spike](/assets/neon-dash/double-jump.png)

*The start screen and double jump. Some menu symbols still appear as strange text.*

![The portal near the end of Level 1](/assets/neon-dash/level-one-portal.png)

![Level 2 with a different background](/assets/neon-dash/level-two.png)

*The Level 1 portal leads to Level 2 with a different background.*

### Goals for October 15 to October 18

- **By October 16:** Test double jumping ten times. Check that a third jump fails and landing restores the extra jump.
- **By October 18:** Try each published level five times and sample both expanded modes. Record unfair obstacles and check both portal types.
- **By October 18:** Test the later tutorial, all three languages, and saved progress after reloading.
- **By October 18:** Compare 800 × 600 and 1280 × 720 screenshots. Check the ground and controls, and fix the strange menu symbols.

I used WebGPT for the game code and Codex to help edit this journal.
