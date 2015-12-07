#include <ButtonEvent.h>

void setup() {
  ButtonEvent.addButton(12,       //button pin
                        onDown,   //onDown event function
                        onUp,     //onUp event function
                        onHold,   //onHold event function
                        1000,     //hold time in milliseconds
                        onDouble, //double event function
                        200);     //double time interval

  Serial.begin(9600);
}

void loop() {
  ButtonEvent.loop();
}

void onDown(ButtonInformation* Sender) {
  Serial.print("Button (pin:");
  Serial.print(Sender->pin);
  Serial.println(") down!");
}

void onUp(ButtonInformation* Sender) {
  Serial.print("Button (pin:");
  Serial.print(Sender->pin);
  Serial.println(") up!");
}

void onHold(ButtonInformation* Sender) {
  Serial.print("Button (pin:");
  Serial.print(Sender->pin);
  Serial.print(") hold for ");
  Serial.print(Sender->holdMillis);
  Serial.println("ms!");
}

void onDouble(ButtonInformation* Sender) {
  Serial.print("Button (pin:");
  Serial.print(Sender->pin);
  Serial.print(") double click in ");
  Serial.print(Sender->doubleMillis);
  Serial.println("ms!");
}
