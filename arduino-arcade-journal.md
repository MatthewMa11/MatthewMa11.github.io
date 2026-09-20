---
layout: default
title: Arduino Arcade — Development Journal
permalink: /arduino-arcade-journal/
---

[← Back to my homepage](https://MatthewMa11.github.io/)

# Arduino Arcade — Development Journal

**Project:** Tech Seminar mini arcade  
**Journal started:** September 20, 2026  
**Latest documented stage:** Enclosure designed and sliced; physical build and gameplay testing still to be recorded.

The goal of this project is to build a small Arduino arcade with a joystick, a 1588BS LED matrix, and a 3D-printed cabinet. This journal records the design changes, problems, checks, and next steps as the project develops.

## Project at a glance

| Part | Current design |
| --- | --- |
| Controller | Arduino Uno |
| Display | Bare 1588BS LED matrix, nominally 38 × 38 mm |
| Input | Common analog joystick module |
| Cabinet | 100 mm wide × 110 mm deep × 145 mm tall |
| Construction | Open back, removable joystick panel, separate display retainer |
| Tools | Autodesk Fusion and Bambu Studio |
| Print setup | Bambu Lab P1S, 0.4 mm nozzle, PLA Basic |

![CAD preview of the current mini arcade enclosure with its joystick panel installed](https://MatthewMa11.github.io/assets/arduino-arcade/arcade_easy_install_preview.png)

*CAD preview of the latest revision. The electronics and LED pattern are visual references, not a demonstration of working hardware.*

## September 20, 2026 — Enclosure development recap

This first entry summarizes the existing project files and verification reports. The revisions below are presented in development order; separate session dates were not recorded in the notes.

### 1. Building the first cabinet

The first enclosure established the arcade shape, a screen tilted approximately 12° back from vertical, and a joystick deck 42 mm above the base. The shell used nominal 3 mm walls. A separate rear cover provided ventilation and cable access, while a retaining frame held the display behind the front opening.

The display pocket was modeled at 38.6 × 38.6 mm for the nominal 38 × 38 mm package. The Uno rested on raised support rails, with insulating mounting pads planned for attachment. The original joystick mounting arrangement used fixed posts underneath the deck.

**Problem encountered:** The first automated build stopped during STEP export with an invalid geometry argument. The finishing script exported the root component, and the later verification report recorded successful STEP, Fusion archive, and STL exports. It also recorded no detected interference among the checked bodies and no timeline issues.

**Takeaway:** Creating valid cabinet geometry and successfully exporting usable manufacturing files are separate checks. Both need to be recorded.

### 2. Opening the back

The next revision removed the rear cover and suppressed its four mounting posts and pilot holes. The cabinet kept the same overall dimensions, with an open rear aperture of approximately 94 × 139 mm.

This change gives access for wiring, assembly, and removing support material. It also removes the rear-cover fasteners from the assembly. The tradeoff is that the electronics remain exposed at the back.

The open-back version was sliced in Bambu Studio with an estimate of **7 hours 20 minutes** and **239.85 g** of filament. That estimate describes the earlier revision, which was later superseded by the removable-panel design.

### 3. Making the joystick easier to install

The latest revision replaces the fixed joystick posts with a removable control panel. The joystick can be fastened to the panel on the workbench before the complete assembly is lowered into the cabinet. Two screws accessible from above secure the panel to the shell.

The panel has **0.3 mm clearance on each edge** and stays flush with the original deck height. Its fasteners use captured standard metal hex nuts, so this assembly does not depend on printed threads or heat-set inserts.

| Connection | Hardware |
| --- | --- |
| Joystick PCB to panel | Four M2.5 × 8 mm screws and four standard M2.5 hex nuts |
| Panel to cabinet | Two M3 × 8 mm screws and two standard M3 hex nuts |

![Exploded CAD view showing the joystick panel lifted above the cabinet opening](https://MatthewMa11.github.io/assets/arduino-arcade/arcade_joystick_exploded.png)

*The removable panel exposes the joystick screws and nut pockets during assembly. Removing the two panel screws later provides access for servicing.*

**Takeaway:** A component fitting inside the cabinet is only part of the design problem. There also needs to be room to reach its screws, install it, and remove it later.

### 4. Preparing the current print

The current design has three printable parts: the cabinet shell, joystick panel, and display retainer. Reference electronics are excluded from the print. The panel is oriented with its flat top on the build plate and its spacers pointing upward; the display retainer is also laid flat.

| Slicer setting | Saved value |
| --- | --- |
| Printer and nozzle | Bambu Lab P1S, 0.4 mm |
| Material and plate | PLA Basic, textured PEI |
| Layer height | 0.20 mm |
| Walls | 3 |
| Infill | 15% |
| Supports | Automatic tree supports, including supports on model surfaces |
| Estimated total time | 7 hours 25 minutes |
| Estimated filament, including support | 243.95 g |

These are slicer estimates. The saved report says the print was **not sent**, and the project notes do not yet record a completed physical print.

### 5. Checking the design

The saved Fusion report lists three solid printable parts, no active timeline issues, and no detected assembly interference. The STL report records that all three meshes are watertight, with zero nonmanifold edges and zero degenerate triangles.

Those results support moving toward a prototype, but physical fit still needs to be checked. In particular:

- The joystick mounting-hole pitch is provisionally **22 × 26 mm** and must match the actual module.
- The display-body depth, lead clearance, and retainer padding need to be checked against the real display.
- Joystick cap clearance needs to be checked at full tilt and when clicked.
- Nut pockets, panel clearance, and screw lengths need a trial fit after printing.
- Wiring, connectors, and any additional driver board need space that is not fully represented by the simplified electronics models.

## Current status and next steps

- [x] Model the cabinet and reference electronics.
- [x] Revise the cabinet to an open back.
- [x] Add a removable joystick panel with captured nuts.
- [x] Export the latest CAD and printable meshes.
- [x] Record CAD, mesh, and slicer checks.
- [ ] Measure the real components and confirm mounting dimensions.
- [ ] Confirm loaded filament, spool mapping, and build-plate readiness.
- [ ] Print the three parts and record actual time and material use.
- [ ] Remove supports and test the panel, nut pockets, display retainer, and joystick movement.
- [ ] Assemble the electronics and document the wiring and display-driving arrangement.
- [ ] Add or document the Arduino firmware and selected game.
- [ ] Test gameplay and record results, problems, and revisions.

No Arduino sketch or gameplay test result was present in the files reviewed for this entry. The next journal update should establish the electronics and software status alongside the physical build results.

## Project files and evidence

The latest local design files are `Mini_Arcade_Easy_Install.f3d` and `Mini_Arcade_Easy_Install_Bambu.3mf`. The print meshes are `arcade_easy_install_shell.stl`, `arcade_joystick_panel_flat.stl`, and `arcade_easy_install_retainer_flat.stl`. These filenames identify the current revision; the CAD and print files are not included with this journal page.

The recap was compiled from the project README, assembly notes, revision scripts, and saved verification reports. Copies of the reports supporting the main results are available here:

- [Original cabinet export and interference report](https://MatthewMa11.github.io/assets/arduino-arcade/verification_report.json)
- [Removable-panel dimensions and Fusion checks](https://MatthewMa11.github.io/assets/arduino-arcade/easy_joystick_verification.json)
- [Latest STL mesh checks](https://MatthewMa11.github.io/assets/arduino-arcade/easy_install_mesh_verification.json)
- [Latest Bambu project settings and slice estimate](https://MatthewMa11.github.io/assets/arduino-arcade/easy_install_bambu_verification.json)

## Format for future entries

For each build session, add the date, the goal, what changed, and what was actually tested. Include a photo or screenshot where useful, then record the result, what was learned, and the next action. Keep estimated values separate from measured results so progress is easy to follow.
