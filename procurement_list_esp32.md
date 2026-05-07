# Smart Medication Sorting and Monitoring System
## Component Procurement List — ESP32 Version

---

## Key Differences vs Arduino Uno Version

| Topic | Arduino Uno | ESP32 |
|---|---|---|
| GPIO Pins | 14 digital, very tight | 30+ GPIO, comfortable |
| PCA9685 Servo Driver | Almost mandatory | Optional (ESP32 has 16 PWM channels) |
| Wi-Fi / Bluetooth | Not available | Built-in — enables remote monitoring / mobile alerts |
| I2C Buses | 1 bus (A4/A5) | Multiple buses, flexible |
| TFT Display | Very pin-tight | Comfortable, no tradeoffs |
| Processing Speed | 16 MHz | 240 MHz |
| 3.3V Logic | 5V | 3.3V — some modules need level shifting |

> **Important:** ESP32 operates at 3.3V logic. Most I2C modules (DS3231, TCS34725, LCD, PCA9685)
> work fine at 3.3V. However, TCS3200, some buzzers, and some servo signal lines may need a
> level shifter or voltage divider. TCS34725 (I2C, 3.3V compatible) is strongly preferred over TCS3200 in this setup.

---

## 📋 Full List (Including Components We Already Have)

### 🧠 Microcontroller
| Component | Qty | Notes |
|---|---|---|
| ESP32 Development Board (38-pin, e.g. DOIT ESP32 DevKit V1) | 1 | Main processing unit. Has Wi-Fi + Bluetooth built-in |

### 📡 Sensors & Timing
| Component | Qty | Notes |
|---|---|---|
| TCS34725 Color Sensor | 1 | I2C, 3.3V compatible. Strongly preferred over TCS3200 with ESP32 |
| DS3231 RTC Module | 1 | I2C, 3.3V compatible. Also buy a CR2032 coin cell |
| TCRT5000 Reflective Sensor Module | 2 | Detects bead presence and jam conditions. 3.3V signal output is fine for ESP32 |

### ⚙️ Servos & Driver
| Component | Qty | Notes |
|---|---|---|
| SG90 or MG90S 180° Servo | 8 | 6 active + 2 spares. Do NOT buy 360° continuous rotation |
| PCA9685 16-Channel I2C PWM Servo Driver | 1 | Still recommended for clean wiring, even though ESP32 has enough PWM pins |

### 🖥️ User Interface & Feedback
| Component | Qty | Notes |
|---|---|---|
| 2.8" Touchscreen TFT Display (already owned) | 1 | Can be used comfortably with ESP32 — no pin conflict. SPI interface |
| Active 5V Buzzer | 2 | 1 in use, 1 spare. Use a transistor (e.g. 2N2222) or level shifter if signal is 3.3V |
| Rotary Encoder Module | 1 | For schedule configuration |
| Tactile Push Button | 2 | Back/Mode UI buttons |


### 🔌 Power
| Component | Qty | Notes |
|---|---|---|
| 5V 5A Power Adapter | 1 | For servos only |
| Micro-USB or USB-C Cable (check your ESP32 board) | 1 | For programming and powering ESP32 |

### 🔧 Electronics Basics
| Component | Qty | Notes |
|---|---|---|
| Breadboard | 2 | For prototyping (already have 3) |
| Jumper Wires (M-M, M-F, F-F) | 1 pack each | Already owned |
| 1000µF Capacitor (≥10V) | 2 | Servo power rail stabilization |
| 10kΩ Resistor | 10 | Pull-ups, buttons (already have some) |
| 220Ω Resistor | 10 | Signal lines (already have some) |
| 2N2222 or similar NPN transistor | 2-3 | For driving 5V buzzer from 3.3V ESP32 GPIO |
| CR2032 Coin Cell Battery | 1 | Backup power for DS3231 |

### 🏗️ Mechanical Structure
| Component | Qty | Notes |
|---|---|---|
| PLA Filament or Cardboard/Foam | — | Hopper, sorting channels, compartments, dispensing tray |
| Acrylic Sheet or Plastic Enclosure | 1 | Main chassis |
| M3 Screw/Nut/Standoff Kit | 1 | Mechanical assembly |
| Hot Glue Gun + Sticks | 1 | Quick structural bonding |

### 🔴🟢🔵 Test Materials (Beads)
| Component | Qty | Notes |
|---|---|---|
| Green Wooden Beads 10mm (Hobimos) | 2 packs (~40 pcs) | 1 pack = 20 pcs; need 30+ for testing |
| Red Wooden Beads 10mm (Fiolimore) | 1 pack (~135 pcs) | Sufficient quantity |
| Blue Plastic Beads 10mm (312 Konsept) | 2 packs | Plastic may reflect differently — buy extra |

---

## 🛒 Shopping List — Only What We Need to Buy (ESP32 Version)

> **Already have:** ESP32 DevKit, 3 breadboards, jumper wires (M-M / M-F / F-F), 2.8" touchscreen TFT display,
> resistors (330Ω / 220Ω / 1kΩ / 10kΩ / 100kΩ), LEDs, potentiometer, push buttons, buzzer

| Component | Qty | Notes |
|---|---|---|
| TCS34725 Color Sensor | 1 | I2C, 3.3V compatible |
| DS3231 RTC Module | 1 | + CR2032 coin cell |
| TCRT5000 Sensor Module | 2 | Bead presence / jam detection |
| SG90 or MG90S 180° Servo | 8 | 6 active + 2 spares |
| PCA9685 Servo Driver | 1 | Clean servo wiring |
| Rotary Encoder Module | 1 | Schedule input |
| 5V 5A Power Adapter | 1 | Servo power supply |
| Micro-USB or USB-C Cable | 1 | Check ESP32 board connector type |
| 1000µF Capacitor (≥10V) | 2 | Servo rail stabilization |
| CR2032 Coin Cell Battery | 1 | For DS3231 |
| 2N2222 NPN Transistor | 3 | For buzzer and any 5V-logic components driven from 3.3V GPIO |
| M3 Screw/Nut Kit | 1 | Mechanical assembly |
| Hot Glue Gun | 1 | Structural bonding |
| Green Beads 10mm | 2 packs | Testing |
| Red Beads 10mm | 1 pack | Testing |
| Blue Beads 10mm | 2 packs | Testing |

---

## ✅ ESP32 Advantages for This Project

- **TFT display is now easy to use** — no pin conflict, use the one you already own
- **No PCA9685 pin pressure** — but still recommended for clean servo wiring
- **Faster processing** — FSM and sensor polling will be smoother
- **More GPIO** — room for extra sensors, LEDs, or debug pins

## ⚠️ ESP32 Watchouts

- **3.3V logic** — double-check every module's voltage compatibility before wiring
- **Buzzer** — if using active 5V buzzer with 3.3V GPIO signal, add a 2N2222 transistor driver
- **Servo power** — same rule applies: never power servos from ESP32's 3.3V/5V pins; use the external 5V 5A supply
- **Common GND** — ESP32 GND, servo supply GND, all sensor GNDs must be connected together
- **Arduino IDE works** — install ESP32 board package via Boards Manager; most Arduino libraries are compatible

---

*Supplier suggestions (Turkey): robotistan.com · direnc.net · robolinkmarket.com*
