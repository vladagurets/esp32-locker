#ifndef CONSTANTS_H
#define CONSTANTS_H

// NOT editable
const char* NTP_SERVER = "pool.ntp.org";
const char* OPEN_COMMAND_TEXT = "/open";
const char* WHEN_COMMAND_TEXT = "/when";
const char* AMOUNT_COMMAND_TEXT = "/amount";

const int NUMBER_OF_CURRENT_WEEK_ADDRESS = 0;
const int CURRENT_WEEK_OPENINGS_COUNT_ADDRESS = 2;
const int EEPROM_SIZE = 512;
const int FLASH_LED_MAX_INTENCIVITY = 5;
const int FLASH_LED_PIN = 4;
const int FLASH_PWM_CHANNEL = 0;

// Editable
const int SERVO_PIN = 12; // Analog pin number of servo angle
const int INITIAL_SERVO_ANGLE = 160; // Initial angle of servo motor (it will move to this angle every start)
const int FINAL_SERVO_ANGLE = 50;  // Final angle of servo motor (it will move to this angle after catching of open command)
const int MAX_MESSAGE_AGE = 60; // TTL of every message catched by Telegramm bot. If message is older than this value bot will ignore this command
const int MAX_OPENINGS_PER_WEEK = 2; // Max amount of opens per week
const int DAY_OF_WEEK_TO_RESET = 5; // Number of week start day. 1-7, 5 is a Friday so week in this case starts from Friday and ends at Thursday

#endif
