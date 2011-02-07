#include <LedControl.h>

void setup() {
  LedControl.startBlink(9,1000,500);
  LedControl.startBlink(8,200);
}

void loop() {
  LedControl.loop();
}
