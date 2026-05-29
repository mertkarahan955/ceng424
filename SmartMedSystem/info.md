# Smart Medication Sorting and Monitoring System
## Claude Code Session Briefing

---

## 1. Proje Özeti

CENG424 Embedded Computer Systems dersi final projesi. Arduino Uno tabanlı proposal, ESP32 + TFT ekranlı gerçek implementasyona yükseltildi.

**Temel işlev:**
1. Bulk yüklenen renkli boncukları (ilaç proxy) TCS3200 renk sensörü ile sınıflandır
2. PCA9685 üzerinden servo motorlarla doğru compartment'a yönlendir
3. DS3231 RTC ile belirlenen saatte ilgili compartment'ı aç, boncuğu tray'e bırak
4. TFT touch ekran üzerinden kullanıcı arayüzü sun

---

## 2. Donanım — Mevcut Parçalar

| Parça | Model | Durum |
|---|---|---|
| Mikrodenetleyici | ESP32-WROOM-32U (30-pin DevKit) | ✅ Breadboard'da |
| TFT Ekran | 2.8" ILI9341 240x320 SPI + XPT2046 touch + SD slot | ✅ Çalışıyor |
| Renk Sensörü | TCS3200 (Frekans tabanlı, GPIO) | ✅ Çalışıyor |
| RTC | DS3231 (I2C, 0x68) + CR2032 | ❌ Henüz bağlanmadı |
| PWM Servo Driver | PCA9685 (I2C, 0x40) | ❌ Henüz bağlanmadı |
| Servo Motorlar | MG90S x8 (6 aktif + 2 yedek) | ❌ Henüz bağlanmadı |
| IR Sensör | TCRT5000 x2 | ❌ Henüz bağlanmadı |
| Güç | 12V NOAS adapter + buck converter (12V→5V) | ❌ Henüz bağlanmadı |
| Buzzer | Aktif 5V buzzer + 2N2222 transistor | ❌ Temin edilecek |
| Test materyali | 10mm renkli boncuk (kırmızı, yeşil, mavi) | ✅ Var |

---

## 3. Pin Mapping — ESP32 (Kesinleşmiş)

### Breadboard Layout
```
Breadboard 1 alt:  F G H I J  ← ESP32 üst pinler J satırında
─────────────────────────────────────────────────────────────
Breadboard 2 alt:  F G H I J  ← ESP32 alt pinler F satırında

ESP32 sütun 20-38 arası (USB portu sağa bakıyor)
G, H, I satırları boş → jumper kablo alanı
```

### TFT ILI9341 + XPT2046 (✅ Çalışıyor)

> **⚠️ DİKKAT:** Aşağıdaki tablo debug sonrası kesinleşmiş ÇALIŞAN konfigürasyondur.
> Eski tablodaki CS=GPIO5 / DC=GPIO2 / T_CS=GPIO15 değerleri YANLIŞ — kullanma!

| TFT Pin | ESP32 GPIO | Not |
|---|---|---|
| VCC | **5V** | AMS1117-3.3 regülatör var → 5V zorunlu, 3.3V çalışmıyor |
| GND | GND | |
| LED (backlight) | 3V3 | 3.3V yeterli |
| SDI/MOSI | GPIO 23 | VSPI MOSI |
| SCK | GPIO 18 | VSPI SCK |
| SDO/MISO | GPIO 19 | VSPI MISO |
| CS | **GPIO 15** | GPIO 5 strapping pin olduğu için taşındı |
| DC | **GPIO 27** | GPIO 2 boot LED / strapping pin olduğu için taşındı |
| RESET | GPIO 4 | |
| T_CS | **GPIO 33** | Touch CS (henüz bağlanmadı) |
| T_IRQ | GPIO 32 | Touch interrupt (henüz bağlanmadı, opsiyonel) |
| T_DIN | GPIO 23 | MOSI ile ortak |
| T_CLK | GPIO 18 | SCK ile ortak |
| T_DO | GPIO 19 | MISO ile ortak |

**14-pin konnektör sırası (soldan sağa):** T_IRQ · T_DO · T_DIN · T_CS · T_CLK · SDO(MISO) · LED · SCK · SDI(MOSI) · DC · RESET · CS · GND · VCC

### TCS3200 Renk Sensörü (✅ Çalışıyor)

| TCS3200 Pin | ESP32 GPIO | Not |
|---|---|---|
| VCC | 3.3V | |
| GND | GND | |
| LED | 3.3V | Dahili LED hep açık |
| S0 | GPIO 13 | Frekans ölçeği: S0=HIGH, S1=HIGH → %100 |
| S1 | GPIO 25 | |
| S2 | GPIO 14 | Filtre seçimi (LOW/LOW=K, HIGH/HIGH=Y, LOW/HIGH=M) |
| S3 | GPIO 26 | ⚠️ Buzzer ile çakışıyor — buzzer GPIO16'ya taşınacak |
| OUT | GPIO 32 | pulseIn() ile frekans ölçümü — T_IRQ iptali (opsiyonel) |

**Çalışma mantığı:** Düşük pulseIn değeri = o renk baskın. K < Y ve K < M → Kırmızı.

**Timeout:** 80.000 µs (80ms). 0 dönerse sensör önü boş.

### I2C Bus (Henüz bağlanmadı)

| Cihaz | GPIO | Not |
|---|---|---|
| SDA | GPIO 21 | DS3231 + PCA9685 aynı hatta |
| SCL | GPIO 22 | DS3231 + PCA9685 aynı hatta |

### I2C Adresleri

| Cihaz | Adres |
|---|---|
| DS3231 | 0x68 |
| PCA9685 | 0x40 |

### Diğer GPIO'lar

| Cihaz | GPIO | Not |
|---|---|---|
| TCRT5000 #1 (sensing chamber) | GPIO 34 | Input only, pull-up yok |
| TCRT5000 #2 (dispensing tray) | GPIO 35 | Input only, pull-up yok |
| Buzzer (2N2222 base) | GPIO 16 | GPIO26'dan taşındı (TCS3200 S3 çakışması) |

### PCA9685 Servo Kanal Ataması (Planlanmış)

| Kanal | Servo | Görev |
|---|---|---|
| CH0 | Servo 1 | Rotary sorting chute |
| CH1 | Servo 2 | Compartment A dispensing gate |
| CH2 | Servo 3 | Compartment B dispensing gate |
| CH3 | Servo 4 | Compartment C dispensing gate |
| CH4-5 | Servo 5-6 | Rezerve / hopper feeder |

---

## 4. PlatformIO Konfigürasyonu

### platformio.ini (✅ ÇALIŞAN — kesinleşmiş)
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_port = /dev/cu.usbserial-0001

lib_deps =
    bodmer/TFT_eSPI
    adafruit/Adafruit TCS34725
    adafruit/Adafruit PWM Servo Driver Library
    adafruit/RTClib
    paulstoffregen/XPT2046_Touchscreen
    adafruit/Adafruit BusIO

build_flags =
    -DUSER_SETUP_LOADED
    -DILI9341_DRIVER
    -DTFT_MOSI=23
    -DTFT_MISO=19
    -DTFT_SCLK=18
    -DTFT_CS=15
    -DTFT_DC=27
    -DTFT_RST=4
    -DTOUCH_CS=33
    -DSPI_FREQUENCY=10000000
    -DSPI_READ_FREQUENCY=20000000
    -DSPI_TOUCH_FREQUENCY=2500000
    -DLOAD_GLCD
    -DLOAD_FONT2
    -DLOAD_FONT4
    -DLOAD_FONT6
    -DLOAD_FONT7
    -DLOAD_FONT8
    -DLOAD_GFXFF
    -DSMOOTH_FONT
```

**Kritik notlar:**
- `USER_SETUP_LOADED` build_flag olarak set edildiğinde `User_Setup.h` dosyası **tamamen devre dışı kalır** — tüm konfigürasyon sadece build_flags'ten gelir. User_Setup.h düzenleme — doğru yöntem bu.
- Font'lar (`-DLOAD_FONT2` vb.) `USER_SETUP_LOADED` ile birlikte build_flags'e eklenmek zorunda, yoksa `drawString()` hiçbir şey göstermez.
- SPI frekansı 40MHz → 10MHz düşürüldü (breadboard'da uzun jumper kablolar için daha stabil).
- Upload sırasında "Connecting..." ekranında ESP32 **BOOT butonuna basılı tutulması** gerekebilir.

---

## 5. Yazılım Mimarisi — FSM

```
IDLE
 ├── TCRT5000 #1 trigger → SENSING
 │     └── TCS3200 renk oku → CLASSIFYING
 │           ├── Renk eşleşti → SORTING (PCA9685 servo döndür) → IDLE
 │           └── Eşleşmedi → ERROR (buzzer + ekran) → manuel reset → IDLE
 │
 ├── DS3231 schedule eşleşti → DISPENSING (gate servo aç)
 │     └── TCRT5000 #2 doğrula → ALERT (buzzer + ekran)
 │           └── Kullanıcı onayı → IDLE
 │
 └── Touch ekran → CONFIG (schedule ayarla) → IDLE
```

---

## 6. Renk Sınıflandırma Mantığı

3 renk kategorisi, 3 compartment:

| Renk | Compartment | Servo Açısı |
|---|---|---|
| Kırmızı | A | 0° |
| Yeşil | B | 90° |
| Mavi | C | 180° |

**TCS3200 kalibrasyon notu:** pulseIn() ile frekans ölçülür — düşük değer = o renk baskın. "Hangisi minimum" karşılaştırması ile renk tespiti yapılır. Sensing chamber kapalı (karanlık) olmalı — ambient ışık yeşil kanalını özellikle bozuyor.

---

## 7. Touch UI Ekran Tasarımı (Planlanmış)

ILI9341 240x320, landscape modda kullanılacak (320x240):

```
┌─────────────────────────────────┐
│  Smart Med System          [⚙]  │  ← Header (40px)
├─────────────────────────────────┤
│                                 │
│   Durum: IDLE                   │  ← Status area
│   Saat: 14:32:05               │
│                                 │
│  [  Komp A: 12 boncuk  ]       │  ← Compartment status
│  [  Komp B:  8 boncuk  ]
│  [  Komp C: 15 boncuk  ]       │
│                                 │
├─────────────────────────────────┤
│  [  BAŞLAT  ]  [  SCHEDULE  ]  │  ← Buttons (60px)
└─────────────────────────────────┘
```

---

## 8. Sıradaki Adımlar (Öncelik Sırasıyla)

### ✅ Tamamlanan — TFT ILI9341
- SPI bağlantısı çalışıyor, renk + yazı gösteriyor

### ✅ Tamamlanan — TCS3200 Renk Sensörü
- GPIO 13/25/14/26/32 üzerinden bağlı
- pulseIn ile K/Y/M okuma çalışıyor
- TFT ile entegre: boncuk rengini ekranda gösteriyor
- Kalibrasyon: sensing chamber (karanlık kutu) ile iyileştirilecek

### Adım 1 — I2C Tarama
```cpp
// DS3231 + PCA9685 adreslerini doğrula
// Beklenen: 0x40 (PCA9685), 0x68 (DS3231)
```

### Adım 2 — DS3231 RTC Entegrasyonu
- I2C bağlantısı: SDA→GPIO21, SCL→GPIO22
- RTClib kütüphanesi ile saat oku
- Ekranda göster

### Adım 3 — PCA9685 + Servo Test
- I2C bağla
- CH0'dan başlayarak her servoyu 0°-90°-180° test et
- Servo PWM: min=150, max=600 (SG90/MG90S için)

### Adım 4 — TCRT5000 Bağlantısı
- GPIO34 ve GPIO35 (input only)
- TCRT5000 modülleri dahili pull-up içeriyor
- Digital okuma ile presence detection test et

### Adım 5 — FSM Entegrasyonu
- Tüm modüller çalışınca FSM kodunu yaz
- millis() tabanlı non-blocking yapı kullan
- FreeRTOS task kullanma — basit FSM yeterli

### Adım 6 — Touch UI
- TFT_eSPI + XPT2046 ile touch kalibrasyon
- Schedule giriş ekranı
- Durum ekranı

### Adım 8 — Test
- 90 boncuk (30 kırmızı + 30 yeşil + 30 mavi)
- Sorting accuracy > %95 hedef
- 48 saat sürekli çalışma testi

---

## 9. Güç Sistemi (Henüz kurulmadı)

```
220V AC
    └── NOAS 12V 5A adapter
            └── Buck converter (12V → 5V, min 3A)
                    ├── PCA9685 V+ (servo güç rayı)
                    │     └── MG90S x6
                    └── ESP32 VIN (5V tolerant)

NOT: Servo GND ve ESP32 GND ortak olmalı!
NOT: TFT ve sensörler ESP32'nin 3V3 pininden besleniyor, adaptörden değil.
```

---

## 10. Önemli Notlar

- **ESP32 breadboard oturumu kritik:** ESP32 breadboard'a tam oturmazsa tüm SPI pinleri temassız kalır → beyaz ekran. Korkmadan sert bastır.
- **GPIO2:** Boot LED ve strapping pin — TFT DC olarak kullanma. GPIO27 kullan.
- **GPIO5:** Strapping pin — TFT CS olarak kullanma. GPIO15 kullan.
- **TFT VCC 5V zorunlu:** ILI9341 modülünde AMS1117-3.3 regülatör var, 3.3V girişte çalışmıyor.
- GPIO34/35 input-only, dahili pull-up yok — TCRT5000 modülleri kendi pull-up'ını içeriyor
- PCA9685 I2C pull-up: Modülde dahili pull-up yoksa 4.7kΩ x2 ekle
- SPI bus paylaşımı: TFT ve Touch aynı MOSI/MISO/SCK hattını kullanıyor, CS pinleri ile ayrışıyor
- SD kart slotu TFT'de mevcut — schedule verisi buraya kaydedilebilir (opsiyonel)

---

## 11. Debug Session Bulguları (TFT — Çözüldü ✅)

**Problem:** Beyaz ekran — TFT hiçbir renk göstermiyordu.

**Kök neden:** ESP32 breadboard'a tam oturmamıştı. Tüm SPI pinleri (GPIO18/19/23) temassızdı.

**Tetiklenen diğer sorunlar ve çözümleri:**

| Sorun | Sebep | Çözüm |
|---|---|---|
| Beyaz ekran | ESP32 breadboard'da temassız | ESP32'yi sert bastırarak tam oturttuk |
| CS bağlantısı çalışmıyor | GPIO5 strapping pin | GPIO15'e taşındı |
| DC bağlantısı sorunlu | GPIO2 boot LED / strapping | GPIO27'ye taşındı |
| drawString() hiçbir şey göstermiyordu | Font build_flags eksik | -DLOAD_FONT2/4/6/7/8 eklendi |
| VCC'de ekran yanmıyor | 3.3V → AMS1117 besleme sorunu | 5V ray'a taşındı |
| User_Setup.h değişikliği etkisiz | .pio cache'i | `pio run --target clean` |
| Upload port meşgul | Serial Monitor açık | Monitor kapatıldı, sonra upload |
| Upload "Connecting..." takılıyor | ESP32 normal boot | BOOT butonuna basılı tut |

**Sonuç konfigürasyon:** platformio.ini'deki build_flags ile tamamen çalışıyor. User_Setup.h artık devre dışı (USER_SETUP_LOADED).