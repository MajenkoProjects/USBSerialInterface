/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// We need to purge the cache in order for the USB descriptor variables
// to be recompiled in
#pragma parameter purge=true

// We want to rename the board to something more meaningful.
// Note: This only works on the Git version of the chipKIT core!
#pragma parameter extra.flags=-DCDCACM_PROD="USB Serial Bridge"::-DCDCACM_MAN="Majenko Technologies"::-DCDCACM_SER="${uecide:uuid}"

static const int PIN_DTRS = 8;
static const int PIN_DTRP = 12;
static const int PIN_DTRN = 13;

void setup() {
	Serial0.begin(115200);
	Serial.begin(115200);
	pinMode(PIN_DTRS, OPEN);
	pinMode(PIN_DTRP, OUTPUT);
	pinMode(PIN_DTRN, OUTPUT);
	pinMode(PIN_LED1, OUTPUT);
	digitalWrite(PIN_DTRS, HIGH);
	digitalWrite(PIN_DTRP, LOW);
	digitalWrite(PIN_DTRN, HIGH);
	digitalWrite(PIN_LED1, LOW);
}

void loop() {
	static uint32_t led_ts = millis();
	static uint32_t pulse_ts = millis();
	static uint32_t baudRate = 115200;
	
	if (millis() - led_ts > 50) {
		led_ts = millis();
		digitalWrite(PIN_LED1, LOW);
	}

	if (millis() - pulse_ts > 50) {
		pulse_ts = millis();
		digitalWrite(PIN_DTRS, HIGH);
	}

	static boolean connected = false;
	if (Serial) {
		digitalWrite(PIN_DTRP, HIGH);
		digitalWrite(PIN_DTRN, LOW);
		if (!connected) {
			connected = true;
			digitalWrite(PIN_DTRS, LOW);
			pulse_ts = millis();
		}
	} else {
		if (connected) {
			connected = false;
//			digitalWrite(PIN_DTRS, LOW);
//			pulse_ts = millis();
		}
		digitalWrite(PIN_DTRP, LOW);
		digitalWrite(PIN_DTRN, HIGH);
	}

	// If we're not connected, then we don't want to do anything.
	if (!Serial) {
		return;
	}

	uint32_t nbr = Serial.getBaudRate();
	if (nbr != baudRate) {
		baudRate = nbr;
		Serial0.flush();
		Serial0.end();
		Serial0.begin(baudRate);
	}
	
	if (Serial.available()) {
		Serial0.write(Serial.read());
		digitalWrite(PIN_LED1, HIGH);
		led_ts = millis();
	}
	if (Serial0.available()) {
		if (Serial) {
			Serial.write(Serial0.read());
		}
		digitalWrite(PIN_LED1, HIGH);
		led_ts = millis();
	}

}
