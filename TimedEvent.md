Simple task scheduler.

# Methods #

**TimedEvent.addTimer**(intervalMillis, onEvent)

> Add a new timer and start it immediately.

  * **intervalMillis**: Time interval in miliseconds.
  * **onEvent**: Method called on onEvent events.

**TimedEvent.addTimer**(eventId, intervalMillis, onEvent)

> Add a new timer with an associated ID. In this method the event is not started immediately and must be controled with '**start**(eventId)' and '**stop**(eventId)' methods.

  * **eventId**: Associated event ID.
  * **intervalMillis**: Time interval in miliseconds.
  * **onEvent**: Method called on scheduled events.

**TimedEvent.setInterval**(eventId, intervalMillis)

> Set a new interval for a given timer.

  * **eventId**: Associated event ID.
  * **intervalMillis**: Time interval in miliseconds.

**TimedEvent.start**(eventId)

> Start an event timer.

  * **eventId**: Associated event ID.

**TimedEvent.stop**(eventId)

> Stop an event timer.

  * **eventId**: Associated event ID.

**ButtonEvent.loop**()

> Event handler process loop. Must be added to main '**loop**()' function.

# Events #

  * **onEvent**: Happens once when the interval elapses.

# Example #

> In this example one event (event1) is used to control when the another one (event2) is started and stoped at regular intervals while they send data through Serial Monitor:

```
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
```

> In this example a LED blinks with random on and off time intervals. Demonstrates the setInterval() call.

```
´#include <TimedEvent.h>
+
+#define MAX_TIMEOUT       200
+#define MIN_TIMEOUT       20
+#define INCREMENT         10
+
+#define LED_PIN           13
+#define BLINK_TIMER_ID    1
+
+// LED state
+bool on = false;
+
+void setup() {
+  pinMode( LED_PIN, OUTPUT );
+  TimedEvent.addTimer( BLINK_TIMER_ID, MIN_TIMEOUT, onTimeout );
+  TimedEvent.start( BLINK_TIMER_ID );
+  toggleLed();
+}
+
+void loop() {
+  TimedEvent.loop();
+}
+
+void onTimeout( TimerInformation* info ){
+
+  toggleLed();
+    
+  // Adjust the timeout interval
+  TimedEvent.setInterval( BLINK_TIMER_ID, random( MIN_TIMEOUT, MAX_TIMEOUT ) );
+}
+
+void toggleLed(){
+  if ( on ){
+    digitalWrite( LED_PIN, LOW );
+    on = false;
+  } else {
+    digitalWrite( LED_PIN, HIGH );
+    on = true;
+  }
+}
```
