/*
 Name:		TemperatureSensor.ino
 Created:	2/3/2017 1:29:46 PM
 Author:	ose
*/


#include "CanInterface.h"
#include <mcp_can_dfs.h>
#include <mcp_can.h>
#include <BitArray.h>
#include <ByteBuffer.h>
#include <Utils.h>
#include <Timer.h>
#include <dht.h>
#include <Arduino.h>
#include <SPI.h>

#include "HWConfig.h"
#include "TempSensor.h"



// Log tag
#define INIT_TAG F("INIT")
#define LOOP_TAG F("LOOP")

// SW info
#define SW_REV F("1")
#define SW_INFO                                                                \
  String(F("TemperatureSensor SW Rev ")) + SW_REV + String(F(" built ")) +      \
      F(__DATE__) + String(" ") + F(__TIME__)
///////////////////////////

// Test stuff
boolean ledStatus;
int loops;
unsigned long avgExecutionTime;
Timer t, sec;




// the setup function runs once when you press reset or power the board
void setup() {
	t.start();

	tempsensor.init();


	ledStatus = LOW;
	sec.setDuration(1000).start();
	avgExecutionTime = 0;
	loops = 0;
}

// the loop function runs over and over again until power down or reset
void loop() {
	// Test execution time
	t.start();

	tempsensor.update();

	// loop
	// Test execution time
	avgExecutionTime += t.elapsedTime();
	loops++;

	if (sec.hasFinished()) {
		ledStatus = !ledStatus;
		digitalWrite(RUN_LED, ledStatus);
		Serial.print("Temp: ");
		Serial.print(tempsensor.getTemp());
		sec.start();
	}


	avgExecutionTime = 0;
	loops = 0;

}

