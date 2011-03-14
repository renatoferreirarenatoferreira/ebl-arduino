#include <TimedEvent.h>

#define CONTROLLED_TIMER 1

bool flag = true;

void setup() {
  Serial.begin(9600);

  //event 1
  TimedEvent.addTimer(5000, event1);
  //event 2
  TimedEvent.addTimer(CONTROLLED_TIMER, 500, event2);
  TimedEvent.start(CONTROLLED_TIMER);
}

void loop() {
  TimedEvent.loop();
}

void event1(TimerInformation* Sender) {
  Serial.print("event1: ");
  if (flag=!flag) {
    TimedEvent.start(CONTROLLED_TIMER);
    Serial.println("START!");
  } else {
    TimedEvent.stop(CONTROLLED_TIMER);
    Serial.println("STOP!");
  }
}

void event2(TimerInformation* Sender) {
    Serial.println("event2!!");  
}
