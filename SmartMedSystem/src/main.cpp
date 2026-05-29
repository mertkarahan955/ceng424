#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("SmartMedSystem", 60, 100, 4);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("TFT Calisiyor!", 80, 140, 2);

    Serial.println("Sistem hazir.");
}

void loop() {}
