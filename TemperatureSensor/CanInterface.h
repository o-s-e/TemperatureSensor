// CanInterface.h

#ifndef _CANINTERFACE_h
#define _CANINTERFACE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "CanInterfaceNano.h"
#include "HWConfig.h"

#include <Utils.h>
#include <Timer.h>

//Class containing an enum with all the CanID constants hard-coded
class CanID {
public:
	enum IDs : unsigned short {
		//BMS
		TEMP = 0x40,			//Temperature
		HUMI = 0x60				//Humidity
	};
};

//Can stream packet delay
#define PACKET_DELAY	50

//Can stream wait for a response timeout
#define CMD_TIMEOUT		3000

//Enum containing the possible results of a can stream
enum CanStreamResult {
	SUCCES,
	ERROR,
	TIMEOUT,
	WRONG_ACK,
	ABORT
};

//Received can packet callback typedef
typedef void(*CanEventHandler)(CanFrame&);


class CanInterfaceClass
{
public:
	//Function to call in the setup
	void init(int canSpeed);

	//Fucntion to call in the loop
	void update();

	//Set the function to call when a new packet is received
	void setCanEventCallBack(CanEventHandler);

	//Remove the callback
	void removeCanEventCallBack();

	//Return the number of packets to read
	int available();

	//Return the next received packet
	CanFrame& read();

	//Send a packet through the can bus
	void send(CanID::IDs id, byte* data, byte size);

	//Stream
	//Stream a byte array over can
	CanStreamResult streamOverCan(CanID::IDs canID, const char* openStreamCmd, byte* buffer, int size);

	//Wait for a byte array stream over can
	CanStreamResult waitForStreamOverCan(CanID::IDs canID, const char* openStreamCmd, byte* buffer, int expectedBytes);

private:

	//Can packet
	CanFrame frame;

	//Can analyzer receive buffer
	//ByteBuffer rxBuffer;

	//Packet received callback
	CanEventHandler canEvent;

};

extern CanInterfaceClass canInterface;

#endif

