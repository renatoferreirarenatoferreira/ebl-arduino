#include <Properties.h>

#define PROPERTY_BOOTS 0
#define PROPERTY_TEXT 1

void setup() {
  Serial.begin(9600);
  
  if (Properties.load()) {
    if (Properties.getInt(PROPERTY_BOOTS)<=15) {
      //print data
      Serial.print("PROPERTY_BOOTS: ");
      Serial.println(Properties.getInt(PROPERTY_BOOTS));
      Serial.print("PROPERTY_TEXT (");
      Serial.print(Properties.get(PROPERTY_TEXT)->valueSize);
      Serial.print("): ");
      Serial.write((byte*)Properties.get(PROPERTY_TEXT)->value,Properties.get(PROPERTY_TEXT)->valueSize);
      Serial.println();
    
      //sum PROPERTY_BOOTS and update text
      Properties.set(PROPERTY_BOOTS,Properties.getInt(PROPERTY_BOOTS)+1);
      char* text = (char*)malloc(32);
      sprintf(text,"Boot times: %d...", Properties.getInt(PROPERTY_BOOTS));
      Properties.set(PROPERTY_TEXT,(void*)text,strlen(text));
      Properties.save();
    } else {
      Serial.println("Flushing!");
      
      //flush data
      Properties.flush();
      Properties.save();
    }
  } else {
    Serial.println("Starting!");
    
    //create data
    Properties.set(PROPERTY_BOOTS,1);
    char* text = (char*)malloc(32);
    sprintf(text,"Boot times: %d...", Properties.getInt(PROPERTY_BOOTS));
    Properties.set(PROPERTY_TEXT,(void*)text,strlen(text));
    Properties.save();
  }
}

void loop() {
  
}
