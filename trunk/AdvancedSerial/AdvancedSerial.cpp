/*
  AdvancedSerial.cpp - Event-Based Library for Arduino.
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

#include "AdvancedSerial.h"

AdvancedSerialClass::AdvancedSerialClass() {
	this->bufferCondition = READING_STX;
	this->bufferPosition = 0;
	this->message = (AdvancedSerialMessage*) &this->messageBuffer;
	this->message->payload = (byte*)this->message+sizeof(AdvancedSerialMessage);
}

void AdvancedSerialClass::setReceiver(void (*onReceive)(AdvancedSerialMessage* Message)) {
	this->onReceive = onReceive;
}

void AdvancedSerialClass::send(byte type, byte id, byte size, byte* payload) {
	if (size >= 0 && size <= MESSAGE_MAX_PAYLOAD_SIZE) {
		Serial.write((byte)DELIMITER_STX);
		Serial.write(type);
		Serial.write(id);
		Serial.write(size);
		if (size > 0) Serial.write(payload, size);
		Serial.write((byte)DELIMITER_ETX);
	}
}

void AdvancedSerialClass::send(byte id, byte size, byte* payload) {
	this->send(MESSAGE, id, size, payload);
}

void AdvancedSerialClass::loop() {
	while(Serial.available() > 0) {
		switch(bufferCondition) {
			case READING_STX:
				if ( Serial.read() == DELIMITER_STX) {
					this->bufferCondition = READING_HEADER;
				}
				break;

			case READING_HEADER:
				this->messageBuffer[this->bufferPosition] = Serial.read();
				this->bufferPosition++;
				if ( this->bufferPosition == MESSAGE_HEADER_SIZE ) {
					if (this->message->size == 0) {
						//payload is empty
						this->bufferCondition = READING_ETX;
					} else if (this->message->size > 0 && this->message->size <= MESSAGE_MAX_PAYLOAD_SIZE) {
						//read
						this->bufferPosition += sizeof(this->message->payload);
						this->bufferCondition = READING_PAYLOAD;
					} else {
						//avoid wrong sized messages
						this->bufferCondition = READING_STX;
					}
				}
				break;

			case READING_PAYLOAD:
				this->messageBuffer[this->bufferPosition] = Serial.read();
				this->bufferPosition++;
				if ( this->bufferPosition == (this->message->size+sizeof(AdvancedSerialMessage)) ) {
					this->bufferCondition = READING_ETX;
				}
				break;

			case READING_ETX:
				if ( Serial.read() == DELIMITER_ETX) {
					if (this->message->type == DISCOVERY_REQUEST) {
						this->send(DISCOVERY_RESPONSE, 0, 0, NULL);
					} else if (this->message->type == MESSAGE) {
						this->send(MESSAGE_ACKNOWLEDGE, 0, 0, NULL);

						if (this->onReceive != NULL)
							this->onReceive(this->message);
					}
				}
				this->bufferCondition = READING_STX;
				this->bufferPosition = 0;
				break;
		}

	}
}

AdvancedSerialClass AdvancedSerial;