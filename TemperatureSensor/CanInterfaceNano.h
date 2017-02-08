// CanInterfaceNano.h

#ifndef _CANINTERFACENANO_h
#define _CANINTERFACENANO_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <mcp_can.h>
#include <Utils.h>

#include "HWConfig.h"

#define RX_BUFFER_SIZE		8



typedef struct CanFrame {
	unsigned short ID;
	byte size;
	byte data[8];
}CanFrame;

class CanInterfaceNanoClass {

public:

	void init(int speed);
	void sendFrame(CanFrame& frame);
	int available();
	void readFrame(CanFrame* temp);

	friend void onMcpRxInterrupt();

private:
	
	int rxPos;
	volatile int rxLast;
	MCP_CAN can = MCP_CS_PIN;
	volatile CanFrame rxBuffer[RX_BUFFER_SIZE];


};

extern CanInterfaceNanoClass Can0;

#endif

