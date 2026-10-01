---
layout: default
title: Arduino 3D Scanner — Innovator Journal
permalink: /arduino-3d-scanner-journal/
---

[← Back to my homepage](https://MatthewMa11.github.io/)

# Arduino 3D Scanner — Innovator Journal

Entry 1: Spinning 2D scanner  
Minimum testable prototype

## My goal for this prototype

For the first stage of my 3D scanner project, I am working toward a simple spinning 2D scanner using an Arduino Uno, an SG90 servo, and an ultrasonic distance sensor. My goal is to turn the sensor, measure distance at different angles, and send the readings to a computer.

This is my minimum testable prototype: a small version that lets me check whether movement and distance sensing work together before adding more complexity. The SG90 sweeps back and forth across a limited angle rather than rotating continuously. This first entry records the initial code and the tests I plan to carry out.

## Components and wiring

| Component or signal | Purpose or connection |
| --- | --- |
| Arduino Uno | Controls the scan and sends readings over USB |
| SG90 servo | Turns the sensor through the scanning plane |
| Servo signal | Arduino D9 |
| Ultrasonic sensor TRIG | Arduino D7 |
| Ultrasonic sensor ECHO | Arduino D6 |
| Ground | Sensor and servo supply share Arduino GND |
| Sensor mount and base | Keep the sensor aligned and the scanner steady |

The trigger and echo pins in my code are for an ultrasonic distance sensor. I need to confirm its model and power requirements when wiring it. The servo also needs a suitable power supply and a common ground with the Arduino.

## Development process

I started with a simple sequence: move the servo, wait briefly, measure distance, and print the result. Keeping these steps together makes it easier to understand how each reading relates to the direction of the sensor.

My loop runs from -90° to +90° in 2° steps. Adding 90 to the scan angle converts that range into servo commands from 0° to 180°. This gives 91 measurement positions in a sweep. I will check the actual usable range once the sensor is mounted.

After each move, the code waits 50 milliseconds before taking a reading. That delay is a starting point to test; the sensor must settle before its measurement is useful.

## Code explanation

The Arduino sends a short pulse through the trigger pin, then measures the echo pulse with pulseIn. The calculation multiplies the echo time by 0.0343 and divides by two to estimate distance in centimetres. The division accounts for sound travelling to the object and back.

The program prints each measurement as angle,distance at 115200 baud. These pairs will let me plot a 2D scan on the computer.

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

## Testing and documenting the prototype

My first test will use a flat object at a known distance. I will compare the sensor reading with a ruler measurement, repeat the scan, and check whether changing the object's position changes the readings as expected.

I will also check the servo's return movement. At the end of the sweep, the code commands it back from 180° to 0°. That large movement may need more settling time than the smaller scan steps. A missing echo is another case to check because the current code can print zero when pulseIn times out.

I will document the build with:

- A photo showing the Arduino, servo, sensor, and wiring.
- A photo showing how the sensor is attached to the servo.
- A photo of the test setup with the target and ruler visible.
- A screenshot of the serial readings or first 2D plot, with notes on what worked and what I changed.

Build photos and measured results are still to be added to this entry.

## Reflection and next steps

Reviewing this code helped me understand how a loop can connect motor movement with sensor input. A distance reading becomes more useful when I know the angle at which it was taken. I also learned why timing matters: the servo needs time to move, and the sensor needs time to receive an echo.

This prototype measures one scanning plane. My next step is to test that plane reliably and plot the readings. Later entries will document improvements and the additional controlled movement needed to build toward a 3D scanner.

