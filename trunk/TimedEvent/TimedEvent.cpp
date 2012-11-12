/*
  TimedEvent.cpp - Event-Based Library for Arduino.
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

#include "TimedEvent.h"

TimedEventClass::TimedEventClass() {
	this->count = 0;
}

void TimedEventClass::addTimer(unsigned long intervalMillis, void (*onEvent)(TimerInformation* Sender)) {
	this->addTimer(DEFAULT_TIMER_ID, intervalMillis, onEvent);
	this->currentTimer->enabled = true;
	this->currentTimer->lastEventMillis = millis();
}

void TimedEventClass::addTimer(short eventId, unsigned long intervalMillis, void (*onEvent)(TimerInformation* Sender)) {
	if (this->count > 0) {
		this->timers = (TimerInformation*) realloc(this->timers, sizeof(TimerInformation)*(this->count+1));
	} else {
		this->timers = (TimerInformation*) malloc(sizeof(TimerInformation));
	}

	this->setPosition(this->count);
	this->currentTimer->eventId = eventId;
	this->currentTimer->intervalMillis = intervalMillis; 
	this->currentTimer->onEvent = onEvent;
	this->currentTimer->enabled = false;
	
	this->count++;
}

void TimedEventClass::setPosition(short Position) {
	this->currentTimer = this->timers+Position;
}

bool TimedEventClass::findTimer(short eventId) {
	for (this->index = 0; this->index < this->count; this->index++) {
		this->setPosition(this->index);
		
		if (this->currentTimer->eventId == eventId)
			return true;
	}
	return false;
}

void TimedEventClass::loop() {
	for (this->index = 0; this->index < this->count; this->index++) {
		this->setPosition(this->index);
		
		if (this->currentTimer->enabled) {
			this->lastMillis = millis();
			
			//check for millis() overflow
			if (this->lastMillis < this->currentTimer->lastEventMillis)
			{
				//calculate the time before the overflow plus the current time to obtain the invertal
				if (((((unsigned long)-1) - this->currentTimer->lastEventMillis) + this->lastMillis) >= this->currentTimer->intervalMillis) {
					this->currentTimer->lastEventMillis = this->lastMillis;
					this->currentTimer->onEvent(this->currentTimer);
				}
			} else {
				//proceed as a normal increment
				if (this->currentTimer->lastEventMillis+this->currentTimer->intervalMillis <= this->lastMillis) {
					this->currentTimer->lastEventMillis = this->lastMillis;
					this->currentTimer->onEvent(this->currentTimer);
				}
			}
		}
	}
}

void TimedEventClass::start(short eventId) {
	if (this->findTimer(eventId)) {
		this->currentTimer->enabled = true;
		this->currentTimer->lastEventMillis = millis();
	}
}

void TimedEventClass::stop(short eventId) {
	if (this->findTimer(eventId)) {
		this->currentTimer->enabled = false;
	}
}


TimedEventClass TimedEvent;