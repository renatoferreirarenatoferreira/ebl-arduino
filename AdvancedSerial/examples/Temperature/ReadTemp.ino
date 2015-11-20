#include <SoftwareSerial.h>
#include <AdvancedSerial.h>

#define READ_FROM   A0


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
  
}

void loop() {
  temperaturePayload.value = analogRead( READ_FROM );
  temperaturePayload.millis = millis();

  Serial.println( temperaturePayload.value );
  
  Ser.send( MSG_TEMPERATURE, sizeof( temperaturePayload ), (byte*) &temperaturePayload );
  Ser.loop();
  delay( 1000 );
}
