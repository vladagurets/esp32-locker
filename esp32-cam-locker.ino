#include <ArduinoJson.h>
#include <Preferences.h>
#include <time.h>
#include "CTBot.h"

#include "constants.h"
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
    tgBot.sendMessage(msg.sender.id, msg.text);
  }

  delay(1000);
}
