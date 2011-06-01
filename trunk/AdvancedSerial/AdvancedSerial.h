/*
  AdvancedSerial.h - Event-Based Library for Arduino.
  Copyright (c) 2011, Renato A. Ferreira
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
      * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef AdvancedSerial_h
#define AdvancedSerial_h

#define MESSAGE_HEADER_SIZE 3
#define MESSAGE_MAX_PAYLOAD_SIZE 32

#define READING_STX 0x01
#define READING_HEADER 0x02
#define READING_PAYLOAD 0x03
#define READING_ETX 0x04

#define DELIMITER_STX 0x02
#define DELIMITER_ETX 0x03

#define DEBUG 0x01
#define MESSAGE 0x02
#define MESSAGE_ACKNOWLEDGE 0x03
#define DISCOVERY_REQUEST 0x04
#define DISCOVERY_RESPONSE 0x05

#include <stdlib.h>
#include "WProgram.h"

struct AdvancedSerialMessage {
	byte type;
	byte id;
	byte size;
	byte* payload;
};

class AdvancedSerialClass
{
  public:
    AdvancedSerialClass();
	void setReceiver(void (*onReceive)(AdvancedSerialMessage* Message));
	void send(byte id, byte size, byte* payload);
	void loop();

  private:
	byte bufferCondition;
	byte bufferPosition;
	byte messageBuffer[sizeof(AdvancedSerialMessage)+MESSAGE_MAX_PAYLOAD_SIZE];
	AdvancedSerialMessage* message;
	void (*onReceive)(AdvancedSerialMessage* Message);
	void send(byte type, byte id, byte size, byte* payload);

};

//global instance
extern AdvancedSerialClass AdvancedSerial;

#endif