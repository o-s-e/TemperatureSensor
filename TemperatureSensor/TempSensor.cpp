#include "TempSensor.h"

dht DHT;

void TempSensor::init() {
	lastStateUpdate.setDuration(TEMP_S_TTL).start();
  
}

void TempSensor::update()
{
	if (lastStateUpdate.hasFinished()) {

		int chk = DHT.read22(DHT_PIN);
		switch (chk) {
		case DHTLIB_OK:
			temp = DHT.temperature;
			hum = DHT.humidity;
			break;
		case DHTLIB_ERROR_CHECKSUM:
			break;
		case DHTLIB_ERROR_TIMEOUT:
			break;
		default:
			break;
		}
		Serial.print("foo: ");
		Serial.print(temp);
		//Convert temp floate to bytearray
		byte  t_temp = floatToByte(&temp);
		byte t_hum = floatToByte(&hum);
		canInterface.send(CanID::TEMP, &t_temp , sizeof(t_temp));
		canInterface.send(CanID::HUMI, &t_hum, sizeof(t_hum));
		lastStateUpdate.start();
	}
}

byte TempSensor::floatToByte(float *value) {
	byte * t_value = (byte *) &value;
	return  * t_value;
}

TempSensor tempsensor;
