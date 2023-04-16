#include <ArduinoJson.h>
#include <time.h>
#include "CTBot.h"

#include "constants.h"
#include "secrets.h"
#include "helpers.h"

Servo myservo;

void handleMessage(TBMessage msg) {
  if (msg.sender.id != TELEGRAM_USER_ID) {
    tgBot.sendMessage(msg.sender.id, "403: You are not supposed to do it");

    return;
  }

  if (msg.messageType == CTBotMessageText) {
    if (msg.text == OPEN_COMMAND_TEXT) {
      int currentOpeningsCount = getCurrentWeekMovementsCount();
      int msgAge = (int)difftime(now, msg.date);

      if (msgAge > MAX_MESSAGE_AGE) {
        tgBot.sendMessage(msg.sender.id, "410: command is too old");
        return;
      }

      if (currentOpeningsCount == MAX_OPENINGS_PER_WEEK) {
        tgBot.sendMessage(msg.sender.id, "Weekly limit exceeded...");
      } else {
        tgBot.sendMessage(msg.sender.id, "Opening...");
        setCurrentWeekMovementsCount(currentOpeningsCount + 1);
        openBox();
      }
    } else {
      tgBot.sendMessage(msg.sender.id, "400: Unknown command");
    }
  }
}

void setup() {
  Serial.begin(115200);

  setupServo();
  setupFlashLed();
  setupEEPROM();
  setupTGBot();
  setupTime();

  initState();
}

void loop() {
  TBMessage msg;

  if (CTBotMessageText == tgBot.getNewMessage(msg)) {
    handleMessage(msg);
  }

  delay(1000);
}

