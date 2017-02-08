#ifndef TempSensor_h
#define TempSensor_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif



#include <dht.h>
#include <Timer.h>
#include "HWConfig.h"
#include "CanInterface.h"

// Logger tag
#define T_SEN_TAG F("TempCSensor")




class TempSensor {
public:
  void init();
  void update();
  float getTemp() { return temp; }


private:
	byte floatToByte(float *value);
	//Log timer
	Timer logTimer;
  float temp;
  float hum;

  // TempSensor  TTL timer
  Timer lastStateUpdate;
};

extern TempSensor tempsensor;

#endif
