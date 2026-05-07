# Smart Medication Sorting and Monitoring System
## Component Procurement List

---

## 📋 Full List (Including Components We Already Have)

### 🧠 Microcontroller
| Component | Qty | Notes |
|---|---|---|
| Arduino Uno R3 (CH340 clone acceptable) | 1 | Main processing unit |

### 📡 Sensors & Timing
| Component | Qty | Notes |
|---|---|---|
| TCS34725 Color Sensor | 1 | I2C interface, fewer pins, easier to code. TCS3200 works too but not recommended |
| DS3231 RTC Module | 1 | Real-time clock. Also buy a CR2032 coin cell |
| TCRT5000 Reflective Sensor Module | 2 | Detects bead presence in sensing chamber and jam conditions |

### ⚙️ Servos & Driver
| Component | Qty | Notes |
|---|---|---|
| SG90 or MG90S 180° Servo | 8 | 6 active + 2 spares. Do NOT buy 360° continuous rotation. 2x hopper feed, 1x sorting chute, 3x dispensing gates |
| PCA9685 16-Channel I2C PWM Servo Driver | 1 | Required for managing 6 servos without overloading Arduino pins |

### 🖥️ User Interface & Feedback
| Component | Qty | Notes |
|---|---|---|
| 16x2 I2C LCD Display | 1 | Shows system status and schedule info |
| Active 5V Buzzer | 2 | 1 in use, 1 spare |
| Rotary Encoder Module | 1 | For schedule configuration, much better than plain push buttons |
| Tactile Push Button | 2 | Dedicated Back/Mode buttons for UI |

### 🔌 Power
| Component | Qty | Notes |
|---|---|---|
| 5V 5A Power Adapter | 1 | For servos only. Do not power servos from Arduino |
| USB A-B Cable | 1 | For powering Arduino from PC or separate adapter |

### 🔧 Electronics Basics
| Component | Qty | Notes |
|---|---|---|
| Breadboard | 2 | For prototyping |
| Jumper Wires (M-M, M-F, F-F) | 1 pack each | Mixed types needed for servos and sensors |
| 1000µF Capacitor (≥10V) | 2 | Smooths voltage spikes on servo power rail |
| 10kΩ Resistor | 10 | Pull-ups, buttons |
| 220Ω Resistor | 10 | Signal lines |
| CR2032 Coin Cell Battery | 1 | Backup power for DS3231 RTC |

### 🏗️ Mechanical Structure
| Component | Qty | Notes |
|---|---|---|
| PLA Filament or Cardboard/Foam | — | For hopper, sorting channels, compartments, dispensing tray |
| Acrylic Sheet or Plastic Enclosure | 1 | Main chassis housing |
| M3 Screw/Nut/Standoff Kit | 1 | Mechanical assembly |
| Hot Glue Gun + Sticks | 1 | Quick bonding of structural parts |

### 🔴🟢🔵 Test Materials (Beads)
| Component | Qty | Notes |
|---|---|---|
| Green Wooden Beads 10mm (Hobimos) | 2 packs (~40 pcs) | 1 pack = 20 pcs; need 30+ for testing |
| Red Wooden Beads 10mm (Fiolimore) | 1 pack (~135 pcs) | Sufficient quantity |
| Blue Plastic Beads 10mm (312 Konsept) | 2 packs | Plastic may reflect differently than wood — buy extra |

---

## 🛒 Shopping List — Only What We Need to Buy

> **Already have:** 3 breadboards, jumper wires (M-M / M-F / F-F), 2.8" touchscreen TFT display,
> resistors (330Ω / 220Ω / 1kΩ / 10kΩ / 100kΩ), LEDs, potentiometer, push buttons, buzzer

| Component | Qty | Notes |
|---|---|---|
| Arduino Uno R3 | 1 | Main microcontroller |
| TCS34725 Color Sensor | 1 | I2C color sensor |
| DS3231 RTC Module | 1 | + CR2032 coin cell |
| TCRT5000 Sensor Module | 2 | Bead presence / jam detection |
| SG90 or MG90S 180° Servo | 8 | 6 active + 2 spares |
| PCA9685 Servo Driver | 1 | 6-servo management |
| 16x2 I2C LCD *(optional)* | 1 | Only if not using the TFT display |
| Rotary Encoder Module | 1 | Schedule input |
| 5V 5A Power Adapter | 1 | Servo power supply |
| USB A-B Cable | 1 | Arduino power/programming |
| 1000µF Capacitor (≥10V) | 2 | Servo rail stabilization |
| CR2032 Coin Cell Battery | 1 | For DS3231 |
| M3 Screw/Nut Kit | 1 | Mechanical assembly |
| Hot Glue Gun | 1 | Structural bonding |
| Green Beads 10mm | 2 packs | Testing |
| Red Beads 10mm | 1 pack | Testing |
| Blue Beads 10mm | 2 packs | Testing |

---

## ⚠️ Critical Notes

- **Never power servos from Arduino's 5V pin** — use a separate 5V 5A supply; share a common GND across all components
- **Place TCS34725 inside an enclosed dark sensing chamber** — ambient light will corrupt color readings
- **Design bead gates with 11–12mm clearance** — sized for 10mm beads; tune with tape/cardboard inserts if double-feed occurs
- **Save schedules to Arduino EEPROM** — prevents data loss on power cut; DS3231 battery preserves clock time
- **Check buzzer type:** If it has a "+" marking it is active — connect directly to 5V. If passive, it requires a PWM signal
- **TFT display note:** The 2.8" TFT uses SPI and consumes many pins. Usable if PCA9685 frees up enough pins; otherwise 16x2 I2C LCD is the safer choice

---

*Supplier suggestions (Turkey): robotistan.com · direnc.net · robolinkmarket.com*
