
#include "CanInterfaceNano.h"

void onMcpRxInterrupt() {
	int index = Can0.rxLast;
	byte size;
	byte buffer[8];

	Can0.can.readMsgBuf(&size, buffer);
	Can0.rxBuffer[index].ID = Can0.can.getCanId();
	Can0.rxBuffer[index].size = size;
	for (int i = 0; i <size; i++) {
		Can0.rxBuffer[index].data[i] = buffer[i];
	}

	Can0.rxLast = (Can0.rxLast + 1) % RX_BUFFER_SIZE;;
}

void CanInterfaceNanoClass::init(int speed) {
	pinMode(MCP_RX_INT, INPUT_PULLUP);

	//On arduino set the reset pin also
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
	pinMode(MCP_RST_PINT, OUTPUT);
	digitalWrite(MCP_RST_PINT, HIGH);
#endif

	int res = can.begin(speed);
	if (res == CAN_OK) {
		LOGLN(F("CAN_INIT_OK"));
	}
	else {
		ASSERT(false, F("CAN_INIT_FAIL"));
	}

	SPI.usingInterrupt(MCP_RX_INTERRUPT);
	attachInterrupt(MCP_RX_INTERRUPT, onMcpRxInterrupt, FALLING);

	rxPos = 0;
	rxLast = 0;
}

void CanInterfaceNanoClass::sendFrame(CanFrame& frame) {
	SPI.beginTransaction(SPISettings());
	can.sendMsgBuf(frame.ID, false, frame.size, frame.data);
	Serial.println("Test can send: ");
	Serial.println(frame.ID);
	Serial.println(frame.size);
	SPI.endTransaction();
}

int CanInterfaceNanoClass::available() {
	int count = rxLast - rxPos;
	if (count < 0) count += RX_BUFFER_SIZE;
	return count;
}

void CanInterfaceNanoClass::readFrame(CanFrame* temp) {
	int index = rxPos;
	if (available()) {
		temp->ID = rxBuffer[index].ID;
		temp->size = rxBuffer[index].size;

		for (int i = 0; i < temp->size; i++) {
			temp->data[i] = rxBuffer[index].data[i];
		}

		rxPos = (rxPos + 1) % RX_BUFFER_SIZE;
	}
	else {
		temp->ID = 0;
		temp->size = 0;
	}
}


CanInterfaceNanoClass Can0;