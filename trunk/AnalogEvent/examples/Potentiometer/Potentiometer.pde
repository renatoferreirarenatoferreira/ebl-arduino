#include <AnalogEvent.h>

void setup() {
  AnalogEvent.addAnalogPort(1,        //potentiometer pin
                            onChange, //onChange event function
                            3);       //hysteresis
  
  Serial.begin(9600);
}

void loop() {
  AnalogEvent.loop();
}

void onChange(AnalogPortInformation* Sender) {
  Serial.print("Analog (pin:");
  Serial.print(Sender->pin);
  Serial.print(") changed to: ");
  Serial.print(Sender->value);
  Serial.println("!");
}
