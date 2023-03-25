#include <ArduinoJson.h>
#include <Preferences.h>
#include <time.h>
#include "CTBot.h"

#include "constants.h"
#include "secrets.h"
#include "helpers.h"

void setup() {
  Serial.begin(115200);
  // TODO: add logs for each setup
  setupFlashLed();
  setupEEPROM();
  setupTime();
  setupTGBot();

  initState();
}

void loop() {
  // a variable to store telegram message data
	TBMessage msg;

	if (CTBotMessageText == tgBot.getNewMessage(msg)) {
    handleMessage(msg);
  }

  delay(1000);
}

void handleMessage(TBMessage msg) {
  if (msg.sender.id != TELEGRAM_USER_ID) {
    tgBot.sendMessage(msg.sender.id, "403: You are not supposed to do it");

    return;
  }

  if (msg.messageType == CTBotMessageText) {
    if (msg.text == OPEN_COMMAND_TEXT) {
      int currentOpeningsCount = getCurrentWeekMovementsCount();

      if (currentOpeningsCount == MAX_OPENINGS_PER_WEEK) {
        tgBot.sendMessage(msg.sender.id, "Weekly limit exceeded...");
      } else {
        tgBot.sendMessage(msg.sender.id, "Opening...");
        setCurrentWeekMovementsCount(currentOpeningsCount + 1);
      }
    } else {
      tgBot.sendMessage(msg.sender.id, "400: Unknown command");
    }
  }
}
