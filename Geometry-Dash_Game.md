---
layout: default
title: Geometry Dash My Version
permalink: /geometry-dash-game/
---

[← Back to my homepage](/)

# Geometry Dash My Version

**Innovators Journal · October 13, 2026**

[Play the latest game with music](/assets/neon-dash/neon-dash-v8-music.html)

In class, I used WebGPT to make a game like Geometry Dash. WebGPT wrote the HTML, CSS, and JavaScript. I gave the ideas, tried the results, and asked for fixes.

## Play my game

Click inside the game and choose **Play**. Use **Space**, **↑**, **W**, or click to jump. Release and press again in the air to double jump. In wave mode, hold to rise and release to fall. You can turn music on or off in Settings.

<p><a href="/assets/neon-dash/neon-dash-v8-music.html" target="_blank" rel="noopener">Open the game in a full window ↗</a></p>

<iframe
  src="/assets/neon-dash/neon-dash-v8-music.html"
  title="Play Geometry Dash My Version"
  width="100%"
  height="600"
  style="display:block; width:100%; height:600px; border:0; border-radius:12px; background:#000;"
  allow="autoplay; fullscreen"
  allowfullscreen>
</iframe>

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

The playable game above is now the v8 music version. The screenshots below show the earlier five-level version.

## What I learned

HTML sets up the page, CSS controls its look, and JavaScript runs the rules. Double jumping helped me understand how variables remember things: the game tracks the extra jump and resets it after landing. I also learned that invisible hitboxes decide collisions. A block's top is safe, but its sides can cause a crash.

I now understand more about how HTML, CSS, and JavaScript work together. In HTML, the `<head>` holds page information and styles, and the `<body>` holds visible parts like buttons and the canvas. CSS controls how they look, while JavaScript adds movement and controls. I also learned that AI prompts need clear details. Asking for a UI that is millions of percent better does not tell the AI what to change. I should name specific effects and settings, like buttons that are 20% bigger, a dark blue background, or a jump height of two blocks, so the AI has a clear direction.

### Connecting HTML CSS and JavaScript

Looking at my actual game helped me understand how the three languages connect. In the first examples below, HTML creates the canvas and gives it the ID `game`. CSS makes the canvas fill the screen. JavaScript uses `document.getElementById('game')` to find that same element, then `getContext('2d')` gets the tools for drawing the player and obstacles. I learned that the ID connects the HTML element to the JavaScript code. The `<style>` section is where I would change the layout, while the `<script>` section controls the game.

```html
<canvas id="game"></canvas>
```

```css
canvas { display:block; position:fixed; inset:0; width:100%; height:100%; }
```

```javascript
const canvas = document.getElementById('game');
const ctx = canvas.getContext('2d');
```

### Understanding the jump code

The jump code helped me understand variables and conditions. `player.onGround` separates ground jumps from jumps in the air. The `&&` means both checks must be true: there must be a new button press, and `player.airJumps` must be above zero. `player.vy = DJUMP_V` makes the player rise because `DJUMP_V` is `-12.2`. The `--` uses up one extra jump, and `jumpQueued = false` clears that press so it cannot count twice. Landing runs `player.airJumps = 1` to restore the extra jump. Now I can ask AI to change a specific jump setting or rule and understand which part of the code should change.

```javascript
if(player.onGround){
  if(jumpHeld){
    player.vy = JUMP_V; player.onGround = false;
    jumpQueued = false;
  }
} else if(jumpQueued && player.airJumps > 0){
  player.vy = DJUMP_V;
  player.airJumps--;
  jumpQueued = false;
  doubleJumpFX();
}
```

## How I would build software next time

Next time, I would start with one level and test each change before adding more. I would save working versions and ask AI to explain code I do not understand. My prompts would explain what should happen, what went wrong, and how to repeat the problem. AI helped me build the game, but I still needed to decide what worked and what to improve.

## How the game works

The canvas is where the game is drawn. The game loop keeps moving the player, checking for hits, and drawing the next picture. Variables remember the level, attempts, and whether an extra jump is available.

The game checks invisible hitboxes to decide when the player hits a spike or block. Landing on top of a block is safe, but hitting its side causes a crash. The attempt counter tracks crashes, and the progress bar shows how close the player is to the portal.

The game features a Geometry Dash-style game, though it has its own differences. My version of Geometry Dash have double 

## Screenshots and goals

![Game start screen with jump controls](/assets/neon-dash/menu.png)

![The square double jumps above a spike](/assets/neon-dash/double-jump.png)

*The start screen and double jump. Some menu symbols still appear as strange text.*

![The portal near the end of Level 1](/assets/neon-dash/level-one-portal.png)

![Level 2 with a different background](/assets/neon-dash/level-two.png)

*The Level 1 portal leads to Level 2 with a different background.*

### Goals for October 15 to October 18

- **By October 16:** Fix double jump gravity issue, and create a tutorial page for new players.
- **By October 18:** Try each published level five times and sample both expanded modes. Record unfair obstacles and check both portal types. Adjust the speed so it is neither too fast nor too slow. 
- **By October 18:** Refine the tutorials that I already have and improve upon them further. It forces you to play the tutorial every time you start a game, which is inconvenient for those who already know how to play.
- **By October 18:** Compare 800 × 600 and 1280 × 720 screenshots. Check the ground and controls, and fix the strange menu symbols. Add music to the game, and also add a setting to turn the music on or off.

ChatGPT helped me with the format and theme of this journal.
