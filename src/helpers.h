#include <time.h>
#include <EEPROM.h>
#include "CTBot.h"
#include <ESP32Servo.h>

#include "constants.h"
#include "secrets.h"

#ifndef HELPERS_H
#define HELPERS_H

CTBot tgBot;
Servo servo;

time_t now = time(NULL);

void setupFlashLed() {
  Serial.print("*Start [setupFlashLed]...");
  pinMode(FLASH_LED_PIN, OUTPUT);
  ledcSetup(FLASH_PWM_CHANNEL, 200, 8);
  ledcAttachPin(FLASH_LED_PIN, FLASH_PWM_CHANNEL);
  Serial.print("End [setupFlashLed]\n");
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
    Serial.print("End [setupTGBot]\n");
  else
    Serial.print("*Error [setupTGBot]\n");

  delay(3000);
}


void setupTime() {
  Serial.print("*Start [setupTime]...");
  configTime(0, 0, NTP_SERVER);
  Serial.print("End [setupTime]\n");
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

  Serial.print("End [setupEEPROM]\n");
}

int getFreshNumberOfCurrentWeek() {
  // For unknown reason first time we get time it's always from 1 to 10
  // So we need to make some additional same requests to get correct time
  while(now < 100000) {
    startFlashLed();
    delay(1000);
    now = time(NULL);
  }
  
  struct tm* timeinfo = gmtime(&now);
  int dayOfYear = timeinfo->tm_yday;
  int weekNumber = (dayOfYear / 7) + 1;

  return weekNumber;
}

bool isMoreOrEqualDayToReset() {
  struct tm* timeinfo = gmtime(&now);
  int dayOfWeek = timeinfo->tm_wday;

  return dayOfWeek >= DAY_OF_WEEK_TO_RESET;
}

int getNextResetDayTimestamp() {
  struct tm* timeinfo = gmtime(&now);
  int dayOfWeek = timeinfo->tm_wday;
  int daysToResetDay = abs(dayOfWeek - DAY_OF_WEEK_TO_RESET + (dayOfWeek > DAY_OF_WEEK_TO_RESET ? -7 : 0));
  int secondsToResetDay = daysToResetDay * 24 * 60 * 60;

  return now + secondsToResetDay;
}

char* convertTiemstampToHumanReadable(int timestamp) {
  time_t timeGMT = (time_t)timestamp;
  struct tm* timeinfo = gmtime(&timeGMT);
  char* timeString = asctime(timeinfo);

  Serial.print("Reset date is  ");
  Serial.println(timeString);

  return timeString;
}

void initState() {
  Serial.print("*Start [initState]...\n");
  int currentWeekNumber = getCurrentNumberOfWeek();
  delay(100);
  int freshWeekNumber = getFreshNumberOfCurrentWeek();

  if (freshWeekNumber != currentWeekNumber && isMoreOrEqualDayToReset()) {
    Serial.print("Current week: ");
    Serial.print(currentWeekNumber);
    Serial.print("   Fresh week: ");
    Serial.println(freshWeekNumber);

    setCurrentNumberOfWeek(freshWeekNumber);
    setCurrentWeekMovementsCount(0);

    Serial.println("Current week number is updated!");
  } else {
    Serial.print("Fresh week number is same as current: ");
    Serial.println(currentWeekNumber);
  }

  Serial.print("End [initState]\n");
}

void setupServo() {
  servo.attach(SERVO_PIN);
  servo.write(INITIAL_SERVO_ANGLE);
  delay(1000);
}

void openBox() {
  servo.write(FINAL_SERVO_ANGLE);
  delay(1000);
  ledcWrite(FLASH_PWM_CHANNEL, 0);
}

#endif
