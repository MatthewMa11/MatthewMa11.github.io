---
layout: default
title: Arduino 3D Scanner — Development Journal
permalink: /arduino-3d-scanner-journal/
---

[← Back to my homepage](https://MatthewMa11.github.io/)

# Arduino 3D Scanner — Development Journal

Author: Matthew Ma  
Journal prepared: September 30, 2026  
Project stage: Initial scanning prototype

## My project goal

My goal is to develop a 3D scanner using an Arduino Uno, an SG90 servo, and a distance sensor. I want to explore how a physical measurement can become data that a computer can use to describe an object's shape.

The starting point is a sensor that turns through a range of angles and records the distance in each direction. This lets me connect robotics, programming, and geometry in one project.

**Scope of this journal:** The Arduino sketch below is the supplied first version. The sensor model, construction photos, and measured test results have not been provided, so the wiring is a proposed setup and the testing section records work still to verify. The entries are organized by development stage rather than invented build dates.

## Components and proposed wiring

The trigger and echo signals in my code suggest a trigger/echo ultrasonic distance sensor, such as an HC-SR04. I need to confirm the actual sensor model before assembling the circuit.

| Component | Purpose |
| --- | --- |
| Arduino Uno | Runs the scan and sends measurements over USB serial |
| SG90 servo | Turns the distance sensor through the scan |
| Trigger/echo ultrasonic sensor — model to confirm | Measures the distance in the selected direction |
| Breadboard and jumper wires | Connect the circuit |
| Sensor bracket and stable base | Hold the sensor and support the servo |
| Computer with Arduino IDE | Uploads the sketch and receives serial data |
| Suitable regulated servo supply | Provides power matched to the actual servo's requirements |

| Signal | Connection specified by the sketch |
| --- | --- |
| Sensor TRIG | Arduino D7 |
| Sensor ECHO | Arduino D6 |
| Servo signal | Arduino D9 |
| Sensor GND | Arduino GND |
| Servo supply GND | Common ground with Arduino GND |

Power connections depend on the confirmed components. If I use an HC-SR04, its VCC connects to 5 V. I will check the SG90's supply requirements and use an adequate servo supply, with a common ground, to reduce the chance of resets when the motor moves.

## Stage 1 — Planning the scanning method

I chose an approach that takes one distance measurement at each angle. The sketch sweeps from -90° to +90° in 2° steps, giving 91 measurement positions in each sweep.

The angle used for the scan is different from the command sent to the servo. The expression `a + 90` converts the scan range into servo commands from 0° to 180°. A scan angle of 0° therefore sends a 90° command to the servo.

I need to mount the sensor so this middle position points forward. The software angle is a commanded position; it is not feedback confirming the servo's actual angle. Mounting and calibration will affect the accuracy of the scan.

## Stage 2 — Developing the Arduino program

The program has three main jobs: move the servo, measure distance, and send the result to a computer.

In `setup()`, serial communication starts at 115200 baud. The servo attaches to D9, while D7 is configured as the trigger output and D6 as the echo input.

During each scan step, the servo moves and the program waits 50 milliseconds. The trigger pin then goes LOW for 2 microseconds, HIGH for 10 microseconds, and LOW again. The program uses `pulseIn(echo, HIGH)` to measure the duration of the echo pulse.

The distance calculation is:

```text
distance in centimetres = echo time in microseconds × 0.0343 ÷ 2
```

The code uses an approximate sound speed of 0.0343 centimetres per microsecond. Dividing by two accounts for the sound travelling to the object and back.

Finally, the sketch prints the angle, a comma, and the calculated distance. This creates a simple `angle,distance_cm` format that can later be saved or plotted.

### My initial code

```cpp
#include <Servo.h> 
 
Servo s; 
int trig = 7, echo = 6; 
 
void setup() { 
  Serial.begin(115200); 
  s.attach(9); 
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT); 
} 
 
void loop() { 
  for (int a = -90; a <= 90; a += 2) { 
    s.write(a + 90); 
    delay(50); 
 
    digitalWrite(trig, LOW); 
    delayMicroseconds(2); 
    digitalWrite(trig, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trig, LOW); 
 
    long t = pulseIn(echo, HIGH); 
    float d = t * 0.0343 / 2; 
 
    Serial.print(a); 
    Serial.print(","); 
    Serial.println(d); 
  } 
}
```

## Stage 3 — Understanding the output

Each line represents one direction and its measured distance. For example:

```text
-90,25.40
-88,25.10
-86,24.80
```

These lines are illustrative examples, not recorded measurements from my project. The first value is the scan angle in degrees; the second is the distance in centimetres. The Serial Monitor must use 115200 baud to match the sketch.

For a horizontal scan, I can turn polar measurements into points using:

```text
theta = angle_degrees × pi / 180
x = distance_cm × sin(theta)
y = distance_cm × cos(theta)
```

Here, x is sideways and y is forward from the sensor's scan origin. These equations assume the measurement originates at that origin; a later, more accurate model would account for the sensor's offset from the servo axis.

## Stage 4 — Testing plan

Before claiming a successful scan, I need to collect evidence.

| Test | Method | Evidence to record |
| --- | --- | --- |
| Servo movement | Check the centre and sweep without forcing the mechanism against its stops | Actual usable range and any interference |
| Distance accuracy | Place a flat target at several ruler-measured distances | Reference distance, sensor reading, and error |
| Repeatability | Repeat measurements with a fixed target | Variation between readings |
| Scan alignment | Place a target directly ahead of the centre position | Angle at which the target appears |
| Missing echoes | Test with no suitable target in range | Timeout behaviour and how invalid readings are handled |
| Power stability | Run repeated sweeps | Any resets, jitter, or interrupted serial output |
| Data capture | Save a complete sweep at 115200 baud | 91 angle/distance records and a plotted scan |

No physical test results are recorded yet. I will add photographs, captured data, and observations after testing.

## Stage 5 — Limitations and improvements

### Moving from a scanning plane to 3D

The current sketch measures angle and distance in one plane. This is an early step toward my 3D scanner goal. To capture height as well, I need a controlled second movement, such as a vertical positioning mechanism or a calibrated tilt axis, and a way to record that position alongside each measurement.

A horizontal scan repeated at known heights could produce `angle,distance,height` records. A tilt mechanism would instead require both angles in the coordinate calculation. I would then combine the measurements into a point cloud, taking account of alignment and parts of the object hidden from the sensor.

### Handling missing measurements

The sketch does not supply an explicit timeout to `pulseIn()`. A missing echo can slow the scan, and a timeout returns zero, which the current calculation prints as a zero distance. A future version should use a timeout appropriate to the sensor and treat missing echoes as invalid data.

### Improving movement and data quality

The 50 ms delay is a starting value to test. It may not allow enough settling time for the sensor mount. At the end of each sweep, the next loop commands the servo from 180° back to 0° and again waits only 50 ms. I should test this return movement and consider a slower return or alternating sweep directions.

Other improvements include taking several readings at each angle, filtering outliers, calibrating the centre position, and adding a scan identifier so the computer can separate repeated sweeps. An ultrasonic sensor's beam also limits how finely it can distinguish small features; a 2° command step alone does not guarantee that level of detail.

## Reflection and next steps

This project connects motor control with sensor input. The servo selects a direction, the sensor measures distance, and serial communication transfers the result to the computer. Understanding how those parts work together is the foundation of the scanner.

The main lesson from reviewing this first version is that recording distances is only part of producing a useful model. I also need reliable movement, valid measurements, and known geometry. My next steps are to confirm the sensor, assemble and calibrate the prototype, complete the tests above, and plot a single scan before adding the second movement required for 3D data.

