#include <Arduino.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include <Adafruit_PWMServoDriver.h>

// --- TCS3200 Pin Tanımları ---
#define S0  13
#define S1  25
#define S2  14
#define S3  26
#define OUT 32

// --- Trigger ---
#define TRIGGER_PIN 17   // INPUT_PULLUP — GND'ye dokununca tetiklenir

// --- PCA9685 Servo Ayarları ---
#define SERVO_CH0    0        // Sorting chute servosunun kanalı
#define SERVO_MIN  150        // 0° için pulse (MG90S)
#define SERVO_MAX  500        // 180° için pulse (MG90S)

TFT_eSPI tft = TFT_eSPI();
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);

volatile bool okumaTetiklendi = false;

void IRAM_ATTR triggerISR() {
    okumaTetiklendi = true;
}

int kanalOku(int s2val, int s3val) {
    digitalWrite(S2, s2val);
    digitalWrite(S3, s3val);
    delay(10);
    return pulseIn(OUT, LOW, 80000);
}

// Açıyı PCA9685 pulse değerine çevir
void servoYaz(uint8_t kanal, int aci) {
    int pulse = map(aci, 0, 180, SERVO_MIN, SERVO_MAX);
    pca.setPWM(kanal, 0, pulse);
}

void ekranaYaz(uint32_t renk, String ad, int aci) {
    tft.fillScreen(renk);
    tft.setTextColor(TFT_WHITE, renk);
    tft.drawString(ad, 70, 85, 4);
    char buf[30];
    sprintf(buf, "Servo: %d derece", aci);
    tft.drawString(buf, 60, 140, 2);
}

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22);

    // TCS3200 başlat
    pinMode(S0, OUTPUT); digitalWrite(S0, HIGH);
    pinMode(S1, OUTPUT); digitalWrite(S1, HIGH);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(OUT, INPUT);

    // Trigger
    pinMode(TRIGGER_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(TRIGGER_PIN), triggerISR, FALLING);

    // PCA9685 başlat
    pca.begin();
    pca.setOscillatorFrequency(27000000);
    pca.setPWMFreq(50);
    servoYaz(SERVO_CH0, 0); // başlangıç: 0° (home pozisyon)
    delay(500);

    // TFT başlat
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.drawString("Kablo bekliyor...", 30, 110, 2);
    tft.drawString("GPIO17 -> GND", 60, 135, 2);

    Serial.println("Hazir. GPIO17'yi GND'ye dokun.");
}

void loop() {
    if (!okumaTetiklendi) return;

    okumaTetiklendi = false;
    delay(30);

    int k = kanalOku(LOW,  LOW);
    int y = kanalOku(HIGH, HIGH);
    int m = kanalOku(LOW,  HIGH);

    Serial.printf("Okuma => K:%4d  Y:%4d  M:%4d\n", k, y, m);

    if (k == 0 && y == 0 && m == 0) {
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.drawString("HATA: Sensore", 55, 100, 2);
        tft.drawString("boncuk koy!", 75, 120, 2);
        Serial.println("Hata: Sensore boncuk koyulmamis.");
        return;
    }

    int servoAcisi;
    uint32_t ekranRengi;
    String renkAdi;

    if (k <= y && k <= m) {
        servoAcisi = 0;
        ekranRengi = TFT_RED;
        renkAdi    = "KIRMIZI";
    } else if (y <= k && y <= m) {
        servoAcisi = 90;
        ekranRengi = TFT_GREEN;
        renkAdi    = "YESIL";
    } else {
        servoAcisi = 180;
        ekranRengi = TFT_BLUE;
        renkAdi    = "MAVI";
    }

    ekranaYaz(ekranRengi, renkAdi, servoAcisi);
    delay(500);
    servoYaz(SERVO_CH0, servoAcisi);

    Serial.printf("Sonuc: %s -> Servo %d derece\n", renkAdi.c_str(), servoAcisi);
}
