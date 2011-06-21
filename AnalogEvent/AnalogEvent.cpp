/*
  AnalogEvent.cpp - Event-Based Library for Arduino.
  Copyright (c) 2011, Renato A. Ferreira
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
      * Neither the name of the author nor the
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

#include "AnalogEvent.h"

AnalogEventClass::AnalogEventClass() {
	this->count = 0;
}

void AnalogEventClass::addAnalogPort(short pin, void (*onChange)(AnalogPortInformation* Sender), int hysteresis) {
	if (this->count > 0) {
		this->ports = (AnalogPortInformation*) realloc(this->ports, sizeof(AnalogPortInformation)*(this->count+1));
	} else {
		this->ports = (AnalogPortInformation*) malloc(sizeof(AnalogPortInformation));
	}
	
	this->setPosition(this->count);
	this->currentPort->pin = pin;
	this->currentPort->value = -99; //force the first event change
	this->currentPort->onChange = onChange;
	this->currentPort->hysteresis = hysteresis;

	this->count++;
}

void AnalogEventClass::setPosition(short Position) {
	this->currentPort = this->ports+Position;
}

void AnalogEventClass::loop() {
	for (this->index = 0; this->index < this->count; this->index++) {
		this->setPosition(this->index);
		this->nextValue = analogRead(this->currentPort->pin);
		
		if (this->currentPort->value != this->nextValue) {
			if (this->currentPort->hysteresis > 0) {
				if (this->currentPort->value-this->nextValue >= this->currentPort->hysteresis ||
					this->nextValue-this->currentPort->value >= this->currentPort->hysteresis) {
						this->currentPort->value = this->nextValue;
						if (this->currentPort->onChange != NULL)
							this->currentPort->onChange(this->currentPort); //call event					
					}
			} else {
				this->currentPort->value = this->nextValue;
				if (this->currentPort->onChange != NULL)
					this->currentPort->onChange(this->currentPort); //call event
			}
		}
	}
}

AnalogEventClass AnalogEvent;