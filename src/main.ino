// -------------------------------------------------------------------------------------------
// Basic Master
// -------------------------------------------------------------------------------------------
//
// This creates a simple I2C Master device which when triggered will send/receive a text 
// string to/from a Slave device.  It is intended to pair with a Slave device running the 
// basic_slave sketch.
//
// Pull pin12 input low to send.
// Pull pin11 input low to receive.
//
// This example code is in the public domain.
//
// -------------------------------------------------------------------------------------------

#include <i2c_t3.h>

int enabled = 0;

void setup()
{
    Serial.begin(115200);

    pinMode(LED_BUILTIN,OUTPUT);    // LED
    digitalWrite(LED_BUILTIN,LOW);  // LED off

    // Setup for Master mode, pins 18/19, external pullups, 400kHz, 200ms default timeout
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
    Wire.setDefaultTimeout(200000); // 200ms

    Serial.begin(115200);
}

uint8_t read8(uint8_t addr, uint8_t reg)
{
	uint8_t result;

	Wire.beginTransmission(addr);
	Wire.write(reg);
	Wire.endTransmission();

	Wire.beginTransmission(addr);
	Wire.requestFrom((uint8_t)addr, (uint8_t)1);
	result = Wire.read();
	Wire.endTransmission();

	return (result);
}

void write8(uint8_t addr, uint8_t reg, uint8_t value)
{
	Wire.beginTransmission(addr);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission();
}

void loop()
{
	int i;

	Serial.printf("Begin TPS65218 register dump:\r\n");
	Serial.printf("-----------------------------\r\n");
	
	for (i = 0; i < 0x26; i++) {
		uint8_t value = read8(0x24, i);
		Serial.printf("reg%d = 0x%02x\r\n",  i, value);
	}

	Serial.printf("-----------------------------\r\n");
	delay(1000);

	if (!enabled) {
		Serial.printf("About to enable DCDC3 and DCDC4!\r\n");
		write8(0x24, 0x10, 0x7d ^ 0x11);
		write8(0x24, 0x11, 0x3f);

		write8(0x24, 0x10, 0x7d ^ 0x12);
		write8(0x24, 0x12, 0x13);

		Serial.printf("Done\r\n");
		enabled = 1;
	}
}

extern "C" int main(void)
{
	setup();
	while (1) {
		loop();
		yield();
	}
}

