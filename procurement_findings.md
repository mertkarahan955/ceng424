# Smart Medication Sorting and Monitoring System - Procurement Findings

Date: 2026-05-07

## 2026-05-07 Decision Update

User clarified:

- Final target is a real physical prototype.
- Tinkercad is only a possible pre-final test/demo environment.
- Beads will be red, blue, and green.
- Each color maps to exactly one medication category.
- Beads are double-holed and roughly 6-12 mm diameter.
- Multiple beads may be dispensed in one schedule.
- Priz/wall power is acceptable.
- Servo power can be separate from Arduino power.
- Medication pickup detection is out of scope.
- Ambient light robustness test is not required; enclosed sensing is acceptable.

Updated recommendation:

- Use bulk-feed architecture only if we can build a simple hopper + singulation gate.
- Use SG90 servos if keeping cost low, but buy spares. Hybrid is also reasonable.
- Prefer TCS34725 over TCS3200 if proposal/component fidelity is flexible, because implementation is easier and it uses fewer Arduino pins.
- Avoid mixed bead diameters if possible. Buy one consistent diameter, ideally around 8-10 mm. A gate sized for 12 mm may double-feed 6 mm beads; a gate sized for 6 mm will jam 10-12 mm beads.

## Current Understanding

Final target is a real physical prototype. Tinkercad can be used as a pre-final test/demo environment, but the hardware design should be based on buildable parts.

The physical item will be a colored bead, roughly 6-10 mm diameter, possibly up to 12 mm. The system will distinguish three colors: red, blue, and green. Each color represents one medication category; the same color will not represent multiple medications.

The intended full workflow is:

1. Mixed beads are loaded into a hopper.
2. Beads are isolated one by one in a sensing chamber.
3. TCS3200/TCS34725 reads the bead color under controlled lighting.
4. A sorting mechanism routes the bead into one of three internal storage compartments.
5. A DS3231 RTC tracks morning/noon/evening schedules over the 48-hour test.
6. Separate dispensing mechanisms release the scheduled quantity from each color compartment.
7. LCD + user input configures schedules; buzzer alerts at dispense time.

## Key Architecture Recommendation

Use a decoupled mechanical design:

- One enclosed sensing chamber for color reading.
- One sorting chute or rotating diverter after sensing.
- Three storage compartments, one per color.
- Three independent dispensing mechanisms, one under each compartment.

This keeps color classification meaningful. The dispensing gates do not replace the color sensor, because the system still accepts mixed input and must sort beads into the correct compartments first.

## Bulk Feed Definition

Bulk feed means the user pours a mixed batch of beads into the hopper at once, and the device itself isolates and processes beads one by one.

This is different from manual feed:

- Manual feed: user drops one bead at a time into the chamber.
- Bulk feed: user pours many mixed beads at once; hopper + gate mechanism creates one-at-a-time flow.

For this project, bulk feed is the stronger interpretation of the proposal sentence "medications loaded in bulk are sequentially analyzed." It also creates the main mechanical risk: double-feed and jam.

Recommended simple bulk-feed implementation:

1. A sloped hopper narrows into a single vertical channel.
2. Upper gate opens briefly to let one bead enter the sensing pocket.
3. Upper gate closes to separate the next beads from the chamber.
4. TCS34725/TCS3200 reads color in the enclosed chamber.
5. Lower gate opens and drops the bead into the sorting chute.
6. Lower gate closes and the cycle repeats.

This uses two chamber/feed servos. A one-servo feed gate is possible, but it is more likely to allow two beads into the chamber when bead diameter varies.

## Important Mechanical Warning

A simple "open bottom flap" can dump more than one bead, especially if a compartment contains a vertical column of beads. Since schedules may require "2 red + 1 blue + 1 green", each dispensing output should behave like a small metering mechanism.

Practical low-cost options:

- Minimum version: one servo gate per compartment, with a bead-sized aperture and short open/close pulses. This is easiest but may release 0/2 beads occasionally.
- Better version: one small rotating disk or wheel per compartment with a single bead-sized hole. Each cycle releases one bead. This is more reliable for counting.
- Most reliable but expensive: two-gate escapement per compartment. This likely adds too many servos.

Recommended for this project: use three servo-driven single-bead gates/rotary disks and tune by testing.

## Servo Count Options

### Option A - Semi-Automatic Feed, Lower Cost

- 1 servo: sensing chamber release gate
- 1 servo: 3-position sorting chute
- 3 servos: dispensing gates, one per compartment
- Total: 5 servos

This assumes the user can help feed beads one by one or the hopper can be manually nudged. It is simpler, but weaker against the "bulk-loaded" claim.

### Option B - Automated Sequential Feed, Recommended

- 2 servos: two-gate escapement for hopper/sensing chamber
- 1 servo: 3-position sorting chute
- 3 servos: dispensing gates, one per compartment
- Total: 6 servos

This better matches the proposal's "bulk-loaded items fall sequentially" requirement.

Buy at least +1 spare servo. If budget allows, buy +2 spares.

## Servo Choice

For 6-10 mm lightweight beads, SG90 180-degree servos should be enough mechanically. MG90S 180-degree servos are more robust and slightly stronger, but cost more. Avoid 360-degree continuous rotation servos for gates and chute positioning; these do not hold commanded angles like normal 180-degree servos.

Suggested purchase strategy:

- Budget version: 7-8 x SG90 180-degree servo.
- Safer version: 6-7 x MG90S 180-degree servo.
- Hybrid version: MG90S for hopper/chamber gates and any gate carrying bead-column load; SG90 for sorting chute and low-load dispensing gates.

## Control Electronics Recommendation

Strongly consider adding a PCA9685 16-channel I2C PWM/servo driver.

Reason:

- With 5-6 servos, direct Arduino Uno wiring becomes pin-tight and messy.
- PCA9685 uses I2C, so it shares SDA/SCL with the LCD and DS3231.
- It provides clean servo connectors and makes external servo power distribution easier.

Without PCA9685, Arduino Uno can still work, but pin budget becomes tight:

- TCS3200: about 5 digital pins
- 5-6 servos: 5-6 pins
- buzzer: 1 pin
- IR/object detection: 1 pin
- UI: 3-4 pins if rotary encoder/buttons
- LCD + RTC: I2C on A4/A5

## Object Detection / Jam Detection

There are two different detection needs:

1. Bead present in sensing chamber.
2. Jam or empty feed detection.

Options:

- No extra sensor: rely on the two-servo feed sequence and TCS readings. Cheapest, but less diagnosable.
- TCRT5000 reflective sensor: cheap and easy, but bead color affects reflection, so red/blue/green may behave differently.
- IR break-beam style detection: better for "something blocks the tube" independent of bead color, but may require more mechanical alignment.

Recommended: buy 1-2 cheap TCRT5000 modules for experiments. If it is unstable due to bead colors, the final firmware can still use timeout/manual-jam handling.

Jam handling can be simple: if no valid color reading appears after feed attempt, show "JAM/EMPTY" on LCD, beep, and wait for manual acknowledge.

## Color Sensor Recommendation

If strict proposal compatibility matters, use TCS3200.

If implementation comfort matters more, use TCS34725.

TCS34725 advantages:

- I2C interface, only SDA/SCL plus power/ground.
- Gives raw red/green/blue/clear values directly via library.
- Has IR filtering, which helps reflected color readings.
- Shares the same bus with DS3231, I2C LCD, and PCA9685 without consuming many extra pins.

TCS3200 disadvantages for this build:

- Needs several digital pins for S0-S3 plus output.
- Produces a frequency signal, so firmware must measure pulse timing.
- Simpler examples often use blocking pulse measurement, which conflicts with a clean non-blocking FSM unless handled carefully.

Practical recommendation: use TCS34725, preferably a lower-cost CJMCU-style board placed inside a custom black chamber. The expensive "yuvali" version is nice, but not mandatory if the chamber is enclosed.

## Bead Notes

Double-holed beads are acceptable, but they introduce two mechanical/color issues:

- If the sensor looks directly into a hole, the reading may be darker than the bead color.
- If bead diameters vary from 6 mm to 12 mm, one fixed gate geometry may either jam on large beads or double-feed small beads.

Recommended mitigation:

- Buy red/blue/green beads from the same product family and same listed diameter.
- Prefer opaque/matte beads over translucent or glossy beads.
- Read color multiple times while the bead is stationary in a small pocket, then classify using normalized ratios such as red/(r+g+b), green/(r+g+b), blue/(r+g+b).
- Design the sensing pocket so the bead is close to the sensor but not pressed onto it; 3-8 mm is a practical first target for TCS34725 modules.

## User Interface Recommendation

Avoid a 4x4 keypad unless you also use an I2C expander; it consumes too many pins for an Uno-based system.

Best simple UI:

- Rotary encoder with push button.
- Optional separate Back/Mode button if pin budget allows.

Suggested UI flow:

- Rotate: change selected value.
- Press: confirm / next field.
- Long press: save / return.
- Optional Mode button: switch between Idle, Schedule Setup, Sort Test, Dispense Test.

Persist schedules in Arduino EEPROM so accidental unplugging does not erase configuration. DS3231 battery preserves the clock time; EEPROM preserves schedule settings.

## Power Recommendation

Do not power servos from Arduino Uno 5V.

Use:

- External 5V supply for servos, preferably 5V 5A if using 5-6 servos.
- Arduino can be powered by USB or a separate safe supply.
- All grounds must be common: Arduino GND, servo supply GND, sensor GND, PCA9685 GND.

A 5V 3A supply may work if servos move one at a time and never stall, but 5V 5A gives much better margin.

## Candidate BOM

Core required parts:

| Item | Suggested Qty | Notes |
| --- | ---: | --- |
| Arduino Uno R3 compatible board | 1 | CH340 clone is acceptable if driver setup is okay |
| TCS3200 color sensor, preferably with shroud/socket | 1 + optional spare | TCS34725 is more accurate but changes the proposal component |
| DS3231 RTC module | 1 | Buy compatible coin cell too; check CR2032/LIR2032 charging note |
| 16x2 I2C LCD | 1 | I2C saves pins |
| SG90 or MG90S 180-degree servos | 7-8 | 6 active + spare(s), depending on chosen architecture |
| PCA9685 16-channel servo driver | 1 | Strongly recommended for 5-6 servos |
| Active 5V buzzer | 1-2 | 1 active, 1 spare optional |
| Rotary encoder module | 1 | Preferred schedule input |
| Optional tactile/back/mode button | 1-2 | Use if UI needs a dedicated back/mode |
| TCRT5000 object sensor module | 1-2 | For chamber presence/jam experiments |
| 5V 5A power adapter/supply | 1 | Servo rail |
| Breadboard | 1-2 | Prototyping |
| Jumper wires M-M, M-F, F-F | 1 pack each | Servo/sensor wiring needs mixed connector types |
| 1000 uF capacitor, >=10V | 1-2 | Across servo supply rail near PCA9685 |
| Resistor kit, 220 ohm + 10k | 1 small pack each | Buttons, LEDs, pullups if needed |
| M3 screws/nuts/standoffs or small screw kit | 1 | Mechanism assembly |
| Plastic/cardboard/acrylic box material | 1 set | Chassis and compartments |
| Beads, red/blue/green | 30+ each | Buy uniform size and matte/opaque if possible |

## Current Turkey Supplier References Checked

The links below are examples for availability/specs, not a final cart:

- Arduino Uno R3 CH340 clone: Robotistan lists a CH340 Uno-compatible board with USB cable.
- TCS3200: Robotistan lists both standard and "sensor yuvali" TCS3200 variants; the shrouded/socketed version is preferable for stable measurements.
- DS3231: Robotistan and Direnc list DS3231 RTC modules; some variants may be out of stock. Check battery type and whether the module charges the backup cell.
- 16x2 I2C LCD: Robotistan lists 16x2 I2C LCD modules.
- SG90/MG90S: Robotistan lists both 180-degree SG90 and MG90S. Avoid 360-degree versions.
- PCA9685: Robotistan lists PCA9685 16-channel I2C PWM/servo driver boards.
- TCRT5000: Robotistan lists single and multi-channel TCRT5000 sensors.
- 5V power: Robotistan/Direnc have 5V 3A/5A options; choose based on stock and connector convenience.

## Remaining Decisions

1. Do we commit to automated bulk feed? If yes, buy for 6-servo architecture.
2. Do we choose SG90, MG90S, or hybrid servos?
3. Do we accept PCA9685 as part of the architecture? Recommendation: yes.
4. Should the final color sensor stay exactly TCS3200 for proposal consistency, or can we upgrade to TCS34725 for easier I2C color readings?
5. What exact bead size/material will be bought? The mechanical gates depend on this.
6. Do we need the final report to honestly say "manual jam recovery" instead of pretending jam detection is fully automated?
