#include "arduino.h"

/**
* Contain all the hardware definitions
*/

// loop
#define RUN_LED 13

// Log
#define LOG_SERIAL Serial
#define LOG_SERIAL_BAUD 115200

#define DHT_PIN 7
#define TEMP_S_TTL 5000

#define CAN_SERIAL_BAUD 115200
#define CAN_SPEED CAN_BPS_125K
#define MCP_RX_INT_PIN		2
#define MCP_RX_INTERRUPT	INT0
#define MCP_RST_PINT		7
#define MCP_CS_PIN			10

