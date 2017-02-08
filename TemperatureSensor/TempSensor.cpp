#include "TempSensor.h"

dht DHT;

void TempSensor::init() {
	lastStateUpdate.setDuration(TEMP_S_TTL).start();
	temp = 0.0;


  
}

void TempSensor::update()
{
	if (lastStateUpdate.hasFinished()) {

		int chk = DHT.read22(DHT_PIN);
		switch (chk) {
		case DHTLIB_OK:
			temp = DHT.temperature;
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
		byte * t_temp = (byte *) &temp;
		canInterface.send(CanID::TEMP, t_temp , sizeof(t_temp));
		lastStateUpdate.start();
	}
}

TempSensor tempsensor;
