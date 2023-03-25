#include <time.h>
#include <EEPROM.h>
#include <WiFiClientSecure.h>

#include "constants.h"

#ifndef HELPERS_H
#define HELPERS_H

WiFiClientSecure client;

void setupFlashLed() {
  pinMode(FLASH_LED_PIN, OUTPUT);
  ledcSetup(FLASH_PWM_CHANNEL, 5000, 8);
  ledcAttachPin(FLASH_LED_PIN, FLASH_PWM_CHANNEL);
}

void toggleFlashLed() {
  int brightness;

  for (brightness = 0; brightness <= FLASH_LED_MAX_INTENCIVITY; brightness++) {
    ledcWrite(FLASH_PWM_CHANNEL, brightness);
    delay(10); // Adjust delay for smoother or faster transitions
  }

  // Decrease brightness
  for (brightness = FLASH_LED_MAX_INTENCIVITY; brightness >= 0; brightness--) {
    ledcWrite(FLASH_PWM_CHANNEL, brightness);
    delay(10); // Adjust delay for smoother or faster transitions
  }
}

void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    toggleFlashLed();
    delay(500);
    Serial.print("Connecting to WiFi");
  }

  Serial.println("\nWiFi connected");
}

void setupTime() {
  configTime(0, 0, NTP_SERVER);
}

int getCurrentNumberOfWeek() {
  return EEPROM.read(NUMBER_OF_CURRENT_WEEK_ADDRESS);
}

int getCurrentWeekMovementsCount() {
  return EEPROM.read(CURRENT_WEEK_MOVEMENTS_COUNT_ADDRESS);
}

void setCurrentNumberOfWeek(int value) {
  EEPROM.write(NUMBER_OF_CURRENT_WEEK_ADDRESS, value);
  EEPROM.commit();
  delay(100);
}

void setCurrentWeekMovementsCount(int value) {
  EEPROM.write(CURRENT_WEEK_MOVEMENTS_COUNT_ADDRESS, value);
  EEPROM.commit();
  delay(100);
}

void setupEEPROM() {
  EEPROM.begin(EEPROM_SIZE);

  // FOR TESTING
  // setCurrentNumberOfWeek(0xFF);
  // setCurrentWeekMovementsCount(0xFF);
  //
}

bool isLeapYear(int year) {
  if (year % 4 == 0) {
    if (year % 100 == 0) {
      if (year % 400 == 0) {
        return true;
      }
      else {
        return false;
      }
    }
    else {
      return true;
    }
  }
  else {
    return false;
  }
}

int getFreshNumberOfCurrentWeek() {
  // Get current time as Unix timestamp
  time_t now = time(nullptr);

  while(now < 100000) {
    delay(100);
    now = time(nullptr);
  }

  // Convert Unix timestamp to struct tm object in UTC
  struct tm tm_now;
  gmtime_r(&now, &tm_now);
  
  // Calculate number of current week
  int yday = 365 + isLeapYear(tm_now.tm_year + 1900);
  int week = ((tm_now.tm_yday - yday) / 7) + 1;
  
  return abs(week);
}

void initState() {
  int currentWeekNumber = getCurrentNumberOfWeek();
  delay(100);
  int freshWeekNumber = getFreshNumberOfCurrentWeek();

  Serial.print("Current week: ");
  Serial.println(currentWeekNumber);
  Serial.print("Fresh week: ");
  Serial.println(freshWeekNumber);

  if (currentWeekNumber != freshWeekNumber) {
    setCurrentNumberOfWeek(freshWeekNumber);
    setCurrentWeekMovementsCount(0);
    Serial.println("Current week number is updated!");
  }
}

#endif