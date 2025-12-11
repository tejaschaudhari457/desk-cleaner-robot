🚀 Desk Cleaner Robot / Obstacle Avoidance Tabletop Bot
A smart autonomous robot that avoids obstacles, detects table edges, and navigates safely using Arduino, Ultrasonic Sensor, IR Sensors, and PWM Motor Driver (L293D).

Overview
This project is a Desk Cleaner / Edge-Avoiding Robot built for safe movement on a table surface.
It continuously reads Ultrasonic distance and IR sensors to avoid collisions and prevent falling off edges.
Whenever danger is detected, the robot performs a full emergency maneuver:
STOP → REVERSE → TURN LEFT → MOVE FORWARD

Features
✔ Ultrasonic obstacle detection
✔ IR digital + analog edge detection
✔ Emergency fail-safe system
✔ Smooth PWM motor control
✔ Motor inversion option for wiring flexibility
✔ Complete sensor + motor testing codes included
✔ Serial monitoring for debugging
✔ Modular and well-structured functions

Hardware Used
| Component                     | Quantity |
| ----------------------------- | -------- |
| Arduino Uno / Nano            | 1        |
| HC-SR04 Ultrasonic Sensor     | 1        |
| IR Sensors (Digital + Analog) | 2        |
| L293D Motor Driver IC         | 1        |
| DC Motors                     | 2        |
| Chassis + Wheels              | 1        |
| Power supply                  | 1        |

Pin Connections
| Signal               | Pin |
| -------------------- | --- |
| Right Motor Forward  | 5   |
| Right Motor Backward | 6   |
| Left Motor Forward   | 9   |
| Left Motor Backward  | 10  |
| Signal | Pin |
| ------ | --- |
| TRIG   | 3   |
| ECHO   | 4   |
| Sensor           | Pin |
| ---------------- | --- |
| Right Digital IR | 8   |
| Left Digital IR  | 2   |
| Right Analog IR  | A0  |
| Left Analog IR   | A1  |


How the Robot Works
1️⃣ Sensor Monitoring
The robot continuously scans:
Distance from ultrasonic sensor
IR analog values (0–1023)
IR digital HIGH/LOW state
2️⃣ Danger Detection
Emergency triggers when:
Obstacle ≤ 5 cm
IR analog ≥ 500 (edge detected)
3️⃣ Emergency Action
1. Stop the motors
2. Reverse both motors
3. Turn left
4. Resume forward movement
4️⃣ Normal Operation
If sensors detect no danger → robot moves forward smoothly using PWM.

Project Folder Structure
📦 Desk-Cleaner-Robot
│
├── README.md                 → Complete documentation
│
├── final-code.c              → FINAL stable robot control program
│
├── pre-final.c               → Near-final version (before optimizations)
├── post-final.c              → After-final updates (small tweaks)
│
├── ir+motar.c                → IR sensors + motor testing
├── ir+ultra+motar.c          → Combined IR + Ultrasonic + motor test
│
├── test_IR.c                 → Test IR sensors individually
├── test_ultrasonic.c         → Test HC-SR04 ultrasonic sensor
├── test_N2o-motar.c          → Motor direction + speed testing
│
└── assets/                   → Images, diagrams, videos (optional)

Future Improvements
Add right turn + random turning
Add Bluetooth remote control
Add vacuum/cleaning module
Add OLED display for sensor info
Add AI-based mapping
Add PID motor control
