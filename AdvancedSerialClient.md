Serial client for AdvancedSerial.

# Types #

**MessageTypes**

> Type of AdvancedSerial messages.

  * **DEBUG**: Debug message.
  * **MESSAGE**: Common message.
  * **MESSAGE\_ACKNOWLEDGE**: Message acknowledge.
  * **DISCOVERY\_REQUEST**: Discovery request message.
  * **DISCOVERY\_RESPONSE**: Discovery response message.

**AdvancedSerialMessage**

> Message class.

  * _MessageTypes_ **Type**: Message type.
  * _byte_ **ID**: Message ID.
  * _byte_ **Size**: Payload size.
  * _byte[.md](.md)_ **Payload**: Message payload.

# Methods #

**Close**()

> Close serial port.

**IsConnected**()

> Verify transport connection.

**Flush**()

> Clear communication stream.

**Open**(Port, BaudRate)

> Starts device communication through a serial port.

  * _string_ **Port**: Serial port.
  * _int_ **BaudRate**: Baud rate.

**Send**(Message)

> Send message.

  * AdvancedSerialMessage **Message**: Message to send.

**Send**(Type, ID, Size, Payload)

> Send message.

  * _MessageTypes_ **Type**: Message type.
  * _byte_ **ID**: Mesage ID.
  * _byte_ **Size**: Payload size.
  * _byte[.md](.md)_ **Payload**: Message payload.

**Send**(Type)

> Clear communication stream.

  * _MessageTypes_ **Type**: Message type.

# Events #

  * **MessageReceived**: Occurs when the a message is received.

# Example #

> This part of a C# code is responsible to send control messages to LCDWriter sketch from [AdvancedSerial](http://code.google.com/p/ebl-arduino/wiki/AdvancedSerial). Permit to control the backlight and text shown in LCD display:

```
try
{
    //create object
    AdvancedSerialClient ArduinoClient = new AdvancedSerialClient();
    //connect to device
    ArduinoClient.Open(Arguments[PARAMETER_PORT].ToString(), 115200);

    //command for LED
    if (Arguments.ContainsKey(PARAMETER_BACKLIGHT))
    {
        SerialProtocol.AdvancedSerialMessage BacklightMessage = new SerialProtocol.AdvancedSerialMessage();

        if ((bool)Arguments[PARAMETER_BACKLIGHT])
            BacklightMessage.ID = BACKLIGHT_ON_MESSAGE;
        else
            BacklightMessage.ID = BACKLIGHT_OFF_MESSAGE;

        ArduinoClient.Send(BacklightMessage);
    }

    //command for text
    if (Arguments.ContainsKey(PARAMETER_TEXT))
    {
        SerialProtocol.AdvancedSerialMessage TextMessage = new SerialProtocol.AdvancedSerialMessage();
        TextMessage.ID = TEXT_MESSAGE;
        TextMessage.Payload = new System.Text.ASCIIEncoding().GetBytes(Arguments[PARAMETER_TEXT].ToString());
        TextMessage.Size = (byte)TextMessage.Payload.Length;
        ArduinoClient.Send(TextMessage);
    }
}
catch (Exception ex)
{
    Console.Write("Error: " + ex.Message);
}
```

> The complete source code for this example is available at library source tree.