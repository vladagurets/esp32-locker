# Treasury locker based on ESP32 module, Servo and [CTBot](https://github.com/shurillu/CTBot)

This is a pet project to deal with ESP32 chip and IoT.

Main motivation is to build a small locker box with an ability to controll it over Telegram bot.
Current implementation allows to open box several times per each week (2 times by default).

To achieve rusults I took the following components:
* [ESP32](https://www.espressif.com/en/products/hardware/esp32/overview) module
* [Servo](https://www.adafruit.com/product/169) motor

Pre-conditions:
- Build your own locker. I modeled and printed a casket with an ability to integrate ESP32Cam module & SG92R
- [Setup Telegramm bot and get API token](https://core.telegram.org/bots#how-do-i-create-a-bot)
- Create `./src/secrets.h` files, copy content from `./secrets.example.h` and update to your desired values
- Update `./src/constants.h` to your desired values. Check definitions inside file
- Burn ESP32
- Find your bot in Telegramm app and start chatting

As result you will be able to open your cascet `n` times per week (initially week starts on Friday and ends on Thursday, you can play with configs in `./src/constants.h`)
