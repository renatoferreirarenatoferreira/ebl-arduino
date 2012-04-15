/*
  TimedEvent.h - Event-Based Library for Arduino.
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

#ifndef TimedEvent_h
#define TimedEvent_h

#include <stdlib.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define DEFAULT_TIMER_ID -99

struct TimerInformation {
	short eventId;
	bool enabled;
	unsigned long intervalMillis;
	unsigned long lastEventMillis;
	void (*onEvent)(TimerInformation* Sender);
};

class TimedEventClass
{
  public:
    TimedEventClass();
	void addTimer(short eventId, unsigned long intervalMillis, void (*onEvent)(TimerInformation* Sender));
	void addTimer(unsigned long intervalMillis, void (*onEvent)(TimerInformation* Sender));
	void start(short eventId);
	void stop(short eventId);
	void loop();
	
  private:
	short count;
	short index;
	unsigned long lastMillis;
    TimerInformation* timers;
	TimerInformation* currentTimer;
	void setPosition(short Position);
	bool findTimer(short eventId);
};

//global instance
extern TimedEventClass TimedEvent;

#endif