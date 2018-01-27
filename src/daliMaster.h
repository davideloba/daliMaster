#ifndef __DALI_MASTER__
#define __DALI_MASTER__

#include "daliCommands.h"

#include <stdint.h>
#include <Wire.h>
#include <Arduino.h>

//default I2C address
#define BAUDRATE         						115200
#define MAX_CHARS 							10U

//default I2C address
#define LW14_I2CADR         						0x23U

#define LW14_I2C_DELAY 	    						2000 //us

//mode
#define LW14_MODE_DACP      						0x00 //direct arc power control
#define LW14_MODE_CMD       						0x01

//register addresses
#define LW14_REG_STATUS 						0x00
#define LW14_REG_CMD    						0x01
#define LW14_REG_CONFIG 						0x02
#define LW14_REG_SIGNATURE 						0xF0
#define LW14_REG_ADDR   						0xFE

//register length
#define LW14_REG_STATUS_LENGTH 						1
#define LW14_REG_CMD_LENGTH    						2
#define LW14_REG_CONFIG_LENGTH 						1
#define LW14_REG_SIGNATURE_LENGTH 					6
#define LW14_REG_ADDR_LENGTH   						2

//status register bitset
#define LW14_STATUS_1_BYTE_TELEGRAM 					0
#define LW14_STATUS_2_BYTE_TELEGRAM 					1
#define LW14_STATUS_REPLY_TIMEFRAME 					2
#define LW14_STATUS_VALID_REPLY 					3
#define LW14_STATUS_FRAME_ERROR 					4
#define LW14_STATUS_OVERRUN 						5
#define LW14_STATUS_BUS_BUSY						6
#define LW14_STATUS_BUS_ERROR		 				7

//DALI stuff
#define DALI_BUS_TIMEOUT						2000
#define TWICE								2
#define MAX_LAMPS_NUM							64
#define DELETE_SA							255U
#define DALI_YES							255U
#define DALI_BROADCAST							255U
#define RESET_DELAY							500000
#define ERR_CODE							-1

/*
0x00 STATUS REGISTER bits:
  7 - Bus Error Status, 0 = OK, 1 = Bus fault
  6 - Busy, 0 = ready, 1 = Busy
  5 - Overrun
  4 - Frame Error
  3 - Valid REPLY (reset by reading 0x01)
  2 - Reply Timeframe, <22 Te since last command
  1 - 2 Bytes telegram received (reset by reading 0x01)
  0 - 1 Byte telegram received (reset by reading 0x01)
*/

class DALIMASTER{

private:

	/*
	 *
	 * set dali master device number
	 *
	 */
	void setDevice(uint8_t);

	/*
	 *
	 * send cmd
	 * doubled if needed by dali specification
	 * @addr
	 * @command
	 *
	 */
	bool cmdSend(uint8_t, uint8_t);
	bool cmdSendTwice(uint8_t, uint8_t);


	/*
	*
	* write command to lw14
	* return true if everything is ok
	*
	*/
	bool lw14Write(uint8_t, uint8_t);

	/*
	*
	* read register from lw14
	* fill array
	* return false if something goes wrong
	*
	*@address
	*@length
	*@*data[]
	*
	*/
	bool lw14Read(uint8_t, uint8_t, uint8_t *);


	/*
	 *
	 * i2c read and write functions
	 * device, address, length, buffer arrat
	 *
	 */

	bool i2cRead(uint8_t, uint8_t, uint8_t, uint8_t *);
	bool i2cWrite(uint8_t, uint8_t, uint8_t, uint8_t *);


/*_____________________________public____________________________*/

public:

	uint8_t device;

	bool begin(const int16_t &);

	/*
	 *
	 * scan i2c bus
	 * print device found
	 *
	 * update reference with their number
	 * return true
	 *
	 */
	uint8_t scanI2cBus(uint8_t &);


	  /*
	  *
	  * set a new i2C address to led warrior module 14
	  * and return true if everything is ok
	  *
	  */
	bool setNewAddr(uint8_t);

	/*
	 *
	 * read lw14 register
	 * and fill array with results
	 *
	 * @register address
	 * @data[]
	 *
	 */
	bool regRead(uint8_t, uint8_t *);

	/*
	 *
	 * Dali commands
	 * see dali specification for details
	 *
	 */

	bool directCmd(uint8_t, uint8_t);

	bool indirectCmd(uint8_t, uint8_t);

	bool configCmd(uint8_t, uint8_t);

	bool queryCmd(uint8_t, uint8_t);

	bool specialCmd(uint8_t, uint8_t);

	/*
	*
	* set the correct Dali address
	* the first one is address ???
	* the second one is mode (direct or command) ???
	* and return it back
	*
	*/

	uint8_t setBroadcastAddress(uint8_t);
	uint8_t setGroupAddress(uint8_t, uint8_t);
	uint8_t setShortAddress(uint8_t, uint8_t);

};

#endif
