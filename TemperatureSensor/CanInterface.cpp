// 
// 
// 

#include "CanInterface.h"

void CanInterfaceClass::init(int canSpeed)
{
	Can0.init(canSpeed);
	canEvent = NULL;
}

void CanInterfaceClass::update() {
	//If a frame is available
	if (Can0.available()) {
		//Read
		Can0.readFrame(&frame);

		//If the callback is set 
		if (canEvent != NULL) {
			//Invoke the callback
			canEvent(frame);
		}

		//Send the packet to the can analyzer
	}
}

int CanInterfaceClass::available()
{
	return Can0.available();
}

void CanInterfaceClass::send(CanID::IDs id, byte* data, byte size) {
	//Send frame over can
	frame.ID = id;
	frame.size = size;
	memcpy(frame.data, data, size);
	Can0.sendFrame(frame);

}

CanFrame& CanInterfaceClass::read() {
	//Return next available can frame
	if (Can0.available()) {
		Can0.readFrame(&frame);
	}
}

void CanInterfaceClass::setCanEventCallBack(CanEventHandler cb) {
	this->canEvent = cb;
}

void CanInterfaceClass::removeCanEventCallBack() {
	this->canEvent = NULL;
}

CanStreamResult CanInterfaceClass::streamOverCan(CanID::IDs canID, const char* openStreamCmd, byte* buffer, int size) {
	byte ack;
	int r, q;
	Timer timeOut;
	CanFrame frame;

	//Open the stream over can
	canInterface.send(canID, (byte*)openStreamCmd, strlen(openStreamCmd));

	//Needed packets to send
	q = size / 8;
	r = size % 8;
	ack = getAck(buffer, size);

	//LOGLN(size);
	//LOG_ARR(buffer, size, HEX);

	//Send packets
	for (int i = 0; i < q; i++) {
		canInterface.send(canID, buffer + i * 8, 8);
		delay(PACKET_DELAY);
	}

	//Send remaining bytes
	if (r != 0) {
		canInterface.send(canID, buffer + q * 8, r);
		delay(PACKET_DELAY);
	}

	//Send ack
	canInterface.send(canID, &ack, 1);

	//Init timeout timer
	timeOut.setDuration(CMD_TIMEOUT).start();

	while (!timeOut.hasFinished()) {
		//Read all response frame
		frame = canInterface.read();
		//If the packet has the stream id
		if (frame.ID == canID) {
			//if response is OK
			if (strcmp((const char*)frame.data, "OK") == 0) {
				return SUCCES;
			}
			//if response is ERROR
			else if (strcmp((const char*)frame.data, "ERR") == 0) {
				return ERROR;
			}
		}
	}

	return TIMEOUT;
}

CanStreamResult CanInterfaceClass::waitForStreamOverCan(CanID::IDs canID, const char* openStreamCmd, byte* buffer, int expectedBytes) {
	byte ack;
	int memIndex;
	Timer timeOut;
	CanFrame frame;

	//Open the stream over can
	canInterface.send(canID, (byte*)openStreamCmd, strlen(openStreamCmd));

	//Set timeout timer
	timeOut.setDuration(CMD_TIMEOUT).start();

	memIndex = 0;
	//Wait for can packet
	while (!timeOut.hasFinished() && memIndex < expectedBytes) {
		frame = canInterface.read();
		//Read responses
		if (frame.ID == canID) {
			//On error
			if (strcmp((const char*)frame.data, "ERR") == 0) {
				return ERROR;
			}
			//Save data in the buffer 
			memcpy(
				buffer + memIndex,
				frame.data,
				memIndex + frame.size > expectedBytes ? expectedBytes - memIndex : frame.size	//Buffer over-run check
			);
			memIndex += frame.size;
		}
	}

	//Timeout
	if (memIndex < expectedBytes) {
		return TIMEOUT;
	}
	//Wait for ack
	while (!timeOut.hasFinished()) {
		frame = canInterface.read();
		if (frame.ID == canID && frame.size == 1) {
			ack = frame.data[0];
			if (ack != getAck(buffer, expectedBytes)) {
				//LOGLN((int)ack);
				//LOGLN((int)getAck(buffer, expectedBytes))
				return WRONG_ACK;
			}
			return SUCCES;
		}
	}

	return TIMEOUT;
}

CanInterfaceClass canInterface;

