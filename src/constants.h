#ifndef CONSTANTS_H
#define CONSTANTS_H

const char* NTP_SERVER = "pool.ntp.org";
const char* OPEN_COMMAND_TEXT = "/open";
const int CURRENT_WEEK_OPENINGS_COUNT_ADDRESS = 2;
const int EEPROM_SIZE = 512;
const int FINAL_SERVO_ANGLE = 50;
const int FLASH_LED_MAX_INTENCIVITY = 5;
const int FLASH_LED_PIN = 4;
const int FLASH_PWM_CHANNEL = 0;
const int INITIAL_SERVO_ANGLE = 160;
const int MAX_MESSAGE_AGE = 60; // in seconds
const int MAX_OPENINGS_PER_WEEK = 2;
const int NUMBER_OF_CURRENT_WEEK_ADDRESS = 0;
const int SERVO_PIN = 12;
const int WEEK_START_OFFSET_IN_DAYS = 5; // By default, week starts on Monday

#endif
