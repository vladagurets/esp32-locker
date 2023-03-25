#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <time.h>

#include "constants.h"
#include "helpers.h"

void setup() {
  Serial.begin(115200);

  setupEEPROM();
  setupFlashLed();
  setupWiFi();
  setupTime();

  initState();
}

void loop() {
  delay(1000);
}
