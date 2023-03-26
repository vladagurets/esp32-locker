#include <time.h>
#include <EEPROM.h>
#include "CTBot.h"

#include "constants.h"
#include "secrets.h"

#ifndef HELPERS_H
#define HELPERS_H

CTBot tgBot;

void setupFlashLed() {
  Serial.print("*Start [setupFlashLed]...");
  pinMode(FLASH_LED_PIN, OUTPUT);
  ledcSetup(FLASH_PWM_CHANNEL, 200, 8);
  ledcAttachPin(FLASH_LED_PIN, FLASH_PWM_CHANNEL);
  Serial.print("*End [setupFlashLed]\n");
}

void startFlashLed() {
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

void setupTGBot() {
  Serial.print("*Start [setupTGBot]...");
  tgBot.wifiConnect(WIFI_SSID, WIFI_PASSWORD);
  tgBot.setTelegramToken(TELEGRAM_TOKEN);

  if (tgBot.testConnection())
    Serial.print("*End [setupTGBot]\n");
  else
    Serial.print("*Error [setupTGBot]\n");
}


void setupTime() {
  Serial.print("*Start [setupTime]...");
  configTime(0, 0, NTP_SERVER);
  Serial.print("*End [setupTime]\n");
}

int getCurrentNumberOfWeek() {
  return EEPROM.read(NUMBER_OF_CURRENT_WEEK_ADDRESS);
}

int getCurrentWeekMovementsCount() {
  return EEPROM.read(CURRENT_WEEK_OPENINGS_COUNT_ADDRESS);
}

void setCurrentNumberOfWeek(int value) {
  EEPROM.write(NUMBER_OF_CURRENT_WEEK_ADDRESS, value);
  EEPROM.commit();
  delay(100);
}

void setCurrentWeekMovementsCount(int value) {
  EEPROM.write(CURRENT_WEEK_OPENINGS_COUNT_ADDRESS, value);
  EEPROM.commit();
  delay(100);
}

void setupEEPROM() {
  Serial.print("*Start [setupEEPROM]...");
  EEPROM.begin(EEPROM_SIZE);

  // FOR TESTING
  setCurrentNumberOfWeek(0xFF);
  setCurrentWeekMovementsCount(0xFF);
  //
  Serial.print("*End [setupEEPROM]\n");
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
    startFlashLed();
    delay(1000);
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
  Serial.print("*Start [initState]...\n");
  int currentWeekNumber = getCurrentNumberOfWeek();
  delay(100);
  int freshWeekNumber = getFreshNumberOfCurrentWeek();

  if (currentWeekNumber != freshWeekNumber) {
    Serial.print("Current week: ");
    Serial.print(currentWeekNumber);
    Serial.print("   Fresh week: ");
    Serial.println(freshWeekNumber);

    setCurrentNumberOfWeek(freshWeekNumber);
    setCurrentWeekMovementsCount(0);

    Serial.println("Current week number is updated!");
  } else {
    Serial.println("Fresh week number is same as current");
  }
  Serial.print("*End [initState]\n");
}

#endif
