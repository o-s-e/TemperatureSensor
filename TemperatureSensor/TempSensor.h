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


typedef union {
	float f_value;
	byte value[sizeof(f_value)];
}ByteUnion;


class TempSensor {
public:
  void init();
  void update();
  float getTemp() { return temp.f_value; }


private:
	//Log timer
	Timer logTimer;
  ByteUnion temp;
  ByteUnion hum;

  // TempSensor  TTL timer
  Timer lastStateUpdate;
};

extern TempSensor tempsensor;

#endif
