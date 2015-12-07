#include <AdvancedSerial.h>
#include <LiquidCrystal.h>

#define BACKLIGHT_ON_MESSAGE 0
#define BACKLIGHT_OFF_MESSAGE 1
#define TEXT_MESSAGE 2

//pins
#define PIN_BACKLIGHT 7

//configure lcd
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

void setup() {
  //configure backlight pin
  pinMode(PIN_BACKLIGHT, OUTPUT);
  //configure LCD
  lcd.begin(16, 2);
  //begin serial port with a desirable speed
  Serial.begin(115200);
  //configure receiver callback
  AdvancedSerial.setReceiver(onMessage);
}

void loop() {
  //AdvancedSerial job
  AdvancedSerial.loop();
}

void onMessage(AdvancedSerialMessage* message) {
  switch (message->id) {
    case BACKLIGHT_ON_MESSAGE:
      digitalWrite(PIN_BACKLIGHT, HIGH);
      break;
      
    case BACKLIGHT_OFF_MESSAGE:
      digitalWrite(PIN_BACKLIGHT, LOW);
      break;
      
    case TEXT_MESSAGE:
      lcd.clear();
      for (int i=0; i<message->size; i++) {
        if (i==16) lcd.setCursor(0, 1);
        lcd.write((char)message->payload[i]);
      }
      break;
  }
}
