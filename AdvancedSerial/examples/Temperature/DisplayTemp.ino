#include <SoftwareSerial.h>
#include <AdvancedSerial.h>

SoftwareSerial comms = SoftwareSerial(2,4);
AdvancedSerialClass Ser( comms );

struct {
  uint32_t  millis;
  uint16_t  value;
} temperaturePayload;

typedef enum {
  MSG_TEMPERATURE,
  MSG_HEATER_BUTTON  
} Messages;

void setup() {
  Serial.begin( 115200 );
  comms.begin( 19200 );

  Ser.setReceiver( onMessage );
}

void loop() {
  Ser.loop();
}

void onMessage( AdvancedSerialMessage* message ){
  switch ( message->id ){
    case MSG_TEMPERATURE:
      memcpy( &temperaturePayload, message->payload, message->size );
      Serial.print( temperaturePayload.millis );
      Serial.print( F(" temp=") );
      Serial.println( temperaturePayload.value );
      break;
    case MSG_HEATER_BUTTON:
      // handle this message
      break;
  }
}


