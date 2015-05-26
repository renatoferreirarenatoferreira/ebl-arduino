_Event-Based Library for Arduino_ is a collection of classes that tries to make easier the programming experience with Arduino.

Currently available classes are:

  * **[AdvancedSerial](http://code.google.com/p/ebl-arduino/wiki/AdvancedSerial)**: Serial programming connector. Implements the event _onReceive_.
    * **[AdvancedSerialClient ](http://code.google.com/p/ebl-arduino/wiki/AdvancedSerialClient)**: Serial client for AdvancedSerial. Currently available only for .Net.
  * **[AnalogEvent](http://code.google.com/p/ebl-arduino/wiki/AnalogEvent)**: Event handler for analog ports that can be used to read potentiometers or others sensors. Implements the event _onChange_.
  * **[ButtonEvent](http://code.google.com/p/ebl-arduino/wiki/ButtonEvent)**: Event handler for buttons (tactile switches) on digital or analog (in series with resistors) ports. Implements the events _onUp_, _onDown_, _onHold_ and _onDouble_.
  * **[LedControl](http://code.google.com/p/ebl-arduino/wiki/LedControl)**: Simple LED functions with asynchronous capabilities. Permits turning on, off and dimmer.
  * **[Properties](http://code.google.com/p/ebl-arduino/wiki/Properties)**: Persistent variables handler. Permits to save variables in EEPROM to persists after reboots.
  * **[RTCTimedEvent](http://code.google.com/p/ebl-arduino/wiki/RTCTimedEvent)**: Simple task scheduler. It's currently supporting only DS1307 based RTC circuits while connected to Arduino's I2C ports (analog 4 for SDA and analog 5 for SCL). Implements _onEvent_ event based on RTC time.
  * **[TimedEvent](http://code.google.com/p/ebl-arduino/wiki/TimedEvent)**: Simple task scheduler. Implements _onEvent_ event based on time interval.

Download options:

  * Download lastest package from [download list](http://code.google.com/p/ebl-arduino/downloads/list).
  * Follow [instructions to checkout the SVN repository](http://code.google.com/p/ebl-arduino/source/checkout).


Using these classes:

> Just copy the _ebl-arduino-src_ folder content into Arduino IDE _libraries_ folder. At next time that you open the environment, the new classes will be available in _Sketch/Import library..._ menu, and their examples available in _File/Examples_.


Let us know your user experience:

> For more information, suggestions or contact the author [subscribe to users mailing list](mailto://ebl-arduino-users+subscribe@googlegroups.com) (don't forget to check your junk filter). You can also try the [issues list](http://code.google.com/p/ebl-arduino/issues/list) of this project.
