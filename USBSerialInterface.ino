
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
			digitalWrite(PIN_DTRS, LOW);
			pulse_ts = millis();
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
