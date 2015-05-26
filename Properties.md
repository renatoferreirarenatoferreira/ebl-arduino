Persistent variables handler. Can be used to save configuration data to Arduino EEPROM persistent memory area.

# Methods #

**LedControl.load**()

> Load data from EEPROM.

**LedControl.load**(position)

> Load data from EEPROM starting from a given position.

  * **position**: Position in EEPROM space.

**LedControl.save**()

> Save data to EEPROM.

**LedControl.save**(position)

> Save data to EEPROM starting from a given position.

  * **position**: Position in EEPROM space.

**LedControl.flush**()

> Flush any loaded or added variables from memory.

**LedControl.set**(propertyId, value)

> Set numeric variable.

  * **propertyId**: Variable ID.
  * **value**: Variable value in _int_ or _long_ types.

**LedControl.set**(propertyId, data, dataSize)

> Set data variable. Could be an array, structure or any portion of memory data.

  * **propertyId**: Variable ID.
  * **data**: Pointer to data in memory.
  * **dataSize**: Size of data to be copied.

**LedControl.getInt**(propertyId)

> Get a variable as _int_.

  * **propertyId**: Variable ID.

**LedControl.getLong**(propertyId)

> Get a variable as  _long_.

  * **propertyId**: Variable ID.

**LedControl.get**(propertyId)

> Get a pointer to _PropertyInformation_ that contains the variable information.

  * **propertyId**: Variable ID.

# Comments #

> Do not abuse of '**Properties.save**()' function due to the limited number of EEPROM cycles (about 100.000).

> The '**Properties.flush**()' does not save data to EEPROM. Call '**Properties.save**()' to erase EEPROM after the flush.

# Example #

> In this example two boot counter variables are saved in EEPROM, one in numeric type and another in string type:

```
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
```