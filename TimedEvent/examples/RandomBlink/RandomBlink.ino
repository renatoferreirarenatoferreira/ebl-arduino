/**
 * Blink an LED with random on and off times between MIN_TIMEOUT and MAX_TIMEOUT milliseconds.
 * Demonstrates setInterval() call.
 */
#include <TimedEvent.h>

#define MAX_TIMEOUT       200
#define MIN_TIMEOUT       20
#define INCREMENT         10

#define LED_PIN           13
#define BLINK_TIMER_ID    1

// LED state
bool on = false;

void setup() {
  pinMode( LED_PIN, OUTPUT );
  TimedEvent.addTimer( BLINK_TIMER_ID, MIN_TIMEOUT, onTimeout );
  TimedEvent.start( BLINK_TIMER_ID );
  toggleLed();
}

void loop() {
  TimedEvent.loop();
}

void onTimeout( TimerInformation* info ){

  toggleLed();
    
  // Adjust the timeout interval
  TimedEvent.setInterval( BLINK_TIMER_ID, random( MIN_TIMEOUT, MAX_TIMEOUT ) );
}

void toggleLed(){
  if ( on ){
    digitalWrite( LED_PIN, LOW );
    on = false;
  } else {
    digitalWrite( LED_PIN, HIGH );
    on = true;
  }
}

