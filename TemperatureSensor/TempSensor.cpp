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
			temp.f_value = DHT.temperature;
			hum.f_value = DHT.humidity;
			break;
		case DHTLIB_ERROR_CHECKSUM:
			break;
		case DHTLIB_ERROR_TIMEOUT:
			break;
		default:
			break;
		}
		Serial.print(temp.f_value);

		canInterface.send(CanID::TEMP, temp.value , sizeof(temp.value));
		canInterface.send(CanID::HUMI, hum.value, sizeof(hum.value));
		lastStateUpdate.start();
	}
}




TempSensor tempsensor;
