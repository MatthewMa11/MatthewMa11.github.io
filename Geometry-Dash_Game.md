---
layout: default
title: Geometry Dash My Version
permalink: /neon-dash-journal/
---

[← Back to my homepage](/)

# Geometry Dash My Version

**Innovators Journal entry · October 13, 2026 · Reflection updated October 15, 2026**

[Play Geometry Dash My Version](/assets/neon-dash/neon-dash-v4-doublejump.html) · [Download the original Word journal](/assets/neon-dash/neon-dash-development-journal.docx) · [View the game source](https://github.com/MatthewMa11/MatthewMa11.github.io/blob/main/assets/neon-dash/neon-dash-v4-doublejump.html)

In class, I used WebGPT to develop a browser game inspired by Geometry Dash. I started with a square that jumps over spikes, then guided several revisions after seeing problems and deciding what I wanted to change. The first published version has five levels and a double jump. My later WebGPT conversation produced expanded versions with menus, two modes, language settings, a tutorial, and wave movement.

[Play the published five-level version](/assets/neon-dash/neon-dash-v4-doublejump.html). The screenshots and detailed code walkthrough below show that earlier version. The later versions are documented here from my WebGPT chat export dated October 15; they are separate from the game linked above.

## How I used AI

WebGPT generated the HTML, CSS, and JavaScript. I did not independently write all of the code. My role was to describe the game, respond to the results, identify problems, and choose changes. The conversation shows that I kept directing the project instead of accepting the first file as the finished game.

The prompt excerpts below are copied from my chat export. The descriptions distinguish my feedback from the AI's explanations and the revisions it returned.

## My development process

### October 12 — The first game

I asked WebGPT to create a Geometry Dash style game with jumping, squares, spikes, levels, portals, and background colors that changed between levels. Part of my initial prompt was:

> Be creative with the maps. Add levels to this thing.

WebGPT returned a single HTML file called neon-dash.html. It chose the NEON DASH name and added neon graphics, particle effects, a progress bar, and an attempt counter. I supplied the general idea and rules, while AI made many of the implementation and visual choices.

### October 12 — Reporting the black screen

The first result did not display properly. I wrote:

> it is showing me a black screen. Fix this issue

WebGPT explained that the canvas could be receiving a zero width or height in the preview environment. It pointed to the visible HUD and increasing attempt count as clues that the game loop could still be running even though the game world was invisible. That was the AI's diagnosis, rather than something I had independently measured.

It returned neon-dash-fixed.html with repeated canvas size checks, a ResizeObserver, and a condition that waits for a valid size before updating and drawing. This revision showed me that a page can load while one part of it still fails. It also showed why the environment where a game opens matters.

### October 12 — Changing the difficulty curve

After the display issue, my feedback shifted to how the game played:

> level 1 is too hard, is should be easy first and hard later at a harder level

WebGPT returned neon-dash-v3.html. It described a slower first level with single spikes and wider gaps, followed by levels with more difficult patterns and increasing speed. The base speed changed from 7.2 to 6.0, with separate speed multipliers for later levels.

This was a more useful request than simply saying to improve the game. I identified the problem and the progression I wanted. The revision changed the level layouts and speed, rather than only changing the appearance. I still needed repeated playtesting to judge whether the new difficulty curve felt fair.

### October 12 — Adding double jumping

Next I asked:

> make that you can double jump

WebGPT returned neon-dash-v4-doublejump.html, the version linked on this page. It separated holding the jump button from making a fresh press. Holding the button still allows automatic ground jumps, while releasing and pressing again in the air uses one extra jump. Landing restores that extra jump.

AI also added a ring effect, particles, and an indicator for the available air jump, and adjusted harder levels around the new ability. I requested the mechanic, while AI chose those supporting details. This helped me see that adding an ability can affect input handling, visual feedback, and level design at the same time.

### October 12 — Asking to understand the code

I then asked:

> for the source, comment on each segment of code explaining what it does and how it contributs to the game

I wanted the source to help me learn how the parts contributed to the game. The export does not show a separate answer immediately after this request. In the later overhaul, WebGPT said it had organized the source into 14 commented sections.

### October 14 — Expanding the game

My next large request asked for a starting menu, Dash-Lite and Dash-Extreme with 50 levels each, English, Spanish, and Chinese settings, and a required instruction course. I also asked for a much better interface.

WebGPT returned neon-dash-ultimate.html after retrying a tool session that it said had expired. It described a menu system, level selection, saved progress, language dictionaries, and a tutorial. To support 100 levels, it changed from individually listed maps to a seeded level generator. The intention was that the same mode and level number would produce the same layout each time.

This request made the project much larger. A returned file and an AI feature summary do not establish that all 100 levels are fair or completable. The menus, saved settings, tutorial, and generated levels each need their own checks.

### October 14 — Requesting another movement form

I wanted the portal to change how the player moved, rather than only move to another level. I wrote:

> I want that when you hit the portal, you switch into the flying line- another form, just like the real Geometry Dash- and when you hit a yellow, you win.

WebGPT interpreted the flying line as wave mode and returned neon-dash-ultimate-wave.html. It described a cube section followed by a blue portal that changes the player into a wave arrow. Holding the control makes the arrow rise diagonally; releasing makes it descend. A yellow portal ends the level.

This was a change to the game's rules, not just its graphics. The same control needed to behave differently depending on the player's form. Although I asked for behavior like the original Geometry Dash, I would need to compare and test the result before claiming it exactly matches that game.

### October 14 — The latest pending request

My last message in the export was:

> tge extreme should be 10 times harder

There is no AI reply or new file after this message in the export, so this remains a request rather than a completed revision. “Ten times harder” also needs a clearer definition. I could guide the next revision by specifying speed, corridor width, obstacle spacing, and the difference I want between Lite and Extreme.

![Game start screen with keyboard controls and double jump instructions](/assets/neon-dash/menu.png)

*Figure 1. The start screen shows the controls. Some symbols appear as strange text, like \u26A1. I want to fix that.*

## Results recorded for the published five-level version

The square moves forward on its own, so the player has to time each jump. The game has glowing platforms, spikes, and a portal at the end of each level. A test showed that the Level 1 portal led to Level 2 and the background changed. Hitting a spike added one to the attempt count and restarted the level.

## What I learned

I learned that the game has several parts working together. Button presses start jumps, variables remember what is happening, and the game loop keeps everything moving. I also noticed that squares are only safe to land on from above. Hitting their sides still makes the player lose. I need to be more exact when I explain the rules to WebGPT.

Looking at one HTML file helped me distinguish three jobs: HTML provides elements such as the canvas and buttons, CSS controls their appearance, and JavaScript handles movement and rules. This makes it easier to decide where to look when something needs to change.

The double jump deepened my understanding of variables and conditions. The program has to remember whether the player is on the ground, whether a new jump press happened, and whether an extra jump is still available. Landing restores the extra jump. A simple player action depends on several pieces of information working together.

Collision detection also showed me that drawing an obstacle and deciding whether it was hit are separate tasks. The program uses invisible hitboxes and checks the player's movement to distinguish a safe landing from a crash. Understanding those checks helps me describe a problem more precisely than just saying that jumping feels wrong.

The portal and spike results recorded here are useful checks, but they do not show that every level is balanced or that every interaction works. I need repeated tests before calling the whole game successful.

The later AI explanations introduced me to procedural generation, localization, and saved state. A seeded generator uses rules and a repeatable starting value to build levels, so making more levels also means testing the generator's rules. Language support requires the interface to look up translated text consistently. Saving progress means some information has to remain after the page closes, while information such as the player's current position can reset.

Wave mode also gave me a clearer example of game state. A cube uses gravity and jumps, while a wave uses held or released input to choose its direction. The program needs to know which form is active before deciding what the control does. I am still building my understanding of these larger systems; requesting them is only the first step toward understanding their implementation.

## How I would approach software development in the future

Next time, I would start with one short level, basic movement, and one obstacle. I would check those parts before adding more levels and visual effects. Smaller steps would make it easier to connect a problem to the change that caused it.

When asking AI for help, I would explain what I expected, what actually happened, and how to reproduce the problem. My black screen and difficulty feedback were useful starting points, but I could make future reports more precise by including the browser, level, controls, and steps involved. I would ask AI to explain the relevant code so I could understand the change and check it myself.

I would also save my prompts, versions, screenshots, and test results as I worked. For each revision, I would record the problem, my request, the change, and the result of testing it. That would make my journal more complete and show which decisions I made and which code AI generated.

My goal is to become better at reading, changing, and testing code. AI can help me build a starting point quickly, but I still need to understand the rules, judge the results, and decide what to improve.

## How the published five-level code works

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

### Updated goals for October 15 to October 18

### Test the double jump by October 16

I will test jumping ten times. I will check the first jump, the double jump, and what happens if I try a third jump. I will also check that landing gives the extra jump back. This will help me understand the jump variables.

### Fix the instructions by October 16

WebGPT returned a version with an introductory course after my October 14 request. I will check whether it teaches jumping, double jumping, and wave controls clearly, including whether it appears on a first visit and can be replayed. I will save a screenshot and record anything that needs another revision.

### Check the levels by October 18

I will try each of the five published levels five times and record my attempts and farthest progress. For the expanded version, I will sample early, middle, and late levels in both modes and check the cube to wave transition and yellow victory portal. I will record the mode and level number for any unfair section so that a follow-up prompt can target a specific problem.

### Try two window sizes by October 18

I will try the game at 800 × 600 and 1280 × 720. I will take screenshots of the same level and compare the ground, controls, and how much of the map I can see. This will help me understand how the canvas size affects the game.

### Check languages and saved progress by October 18

I will check the menus and tutorial in English, Spanish, and Chinese for missing text or layout problems. I will also reload the expanded version to check whether the chosen language and unlocked levels are saved as intended. I will keep these results separate from the features described by AI.

## AI assistance with this journal

I used Codex to help organize and write this journal from my existing entry and the WebGPT chat export. The quoted prompts are my messages from that export. WebGPT generated the game revisions; Codex helped explain and document the development history. The exported conversation ends with my request for a harder Extreme mode.
