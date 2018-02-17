#ifndef __DALI_COMMANDS__
#define __DALI_COMMANDS__

/*
 *
 * DALI
 *
 * 1 start bit
 * 8 address bits: 1 individual or group address bit (Y) , 6 address bits, 1 select bit (S)
 * 8 data bits
 * 2 stop bits
 *
 * ADDRESS BYTE:
 * Short or group address            YAAAAAAS
 * Short addresses (from 0 to 63)    0AAAAAAS
 * Group addresses (from 0 to 15)    100AAAAS
 * Broadcast                         1111111S

 * Special command                   101CCCC1
 * Special command                   110CCCC1
 *
 *
 * S: selector bit:
 *   S = ‘0’ direct arc power level following
 *   S = ‘1’ command following (indirect)
 *   Y = ‘0’ short address
 *   Y = ‘1’ group address or broadcast
 *
 * A: significant address bit
 * C: significant command bit
 *
 * examples:
 *
 * Direct arc power control command
 * YAAA AAA0 XXXX XXXX
 *
 * indirect arc power control commands
 * YAAA AAA1 XXXX XXXX (16-31)
 *
 * configuration commands
 * YAAA AAA1 XXXX XXXX (32-128)
 *
 * query commands
 * YAAA AAA1 XXXX XXXX (144-196)
 *
 * special commands
 * 101CCCC1 XXXX XXXX or
 * 110CCCC1 XXXX XXXX
 *
 *
 */

/*
* DALI indect commands (0 ... 31)
* (IEC-60929, E.4.3.3.1.2)
*
*/
#define DALI_CMD_OFF                  0x00
#define DALI_CMD_UP                   0x01
#define DALI_CMD_DOWN                 0x02
#define DALI_CMD_STEP_UP              0x03
#define DALI_CMD_STEP_DOWN            0x04
#define DALI_CMD_MAX_LEVEL            0x05
#define DALI_CMD_MIN_LEVEL            0x06
#define DALI_CMD_STEP_DOWN_AND_OFF    0x07
#define DALI_CMD_ON_AND_STEP_UP       0x08
#define DALI_CMD_ENABLE_DAPC_SEQUENCE 0x09
#define DALI_CMD_GO_TO_SCENE          0x10 //possible values from 0x10 to 0x1F

/*
* ->DALI configuration commands (32 ... 128)
* (IEC-60929, E.4.3.3.2)
*/
#define DALI_CMD_RESET 		32
#define DALI_CMD_STORE_LEVEL	33
#define DALI_CMD_STORE_DTR_SA	128

/*
* ->DALI query commands (144 ... 255)
* (IEC-60929, E.4.3.3.3)
*/
#define DALI_CMD_QUERY_STATUS	  144
#define DALI_CMD_QUERY_BALLAST	 145
#define DALI_CMD_QUERY_LAMP_FAILURE	146
#define DALI_CMD_QUERY_MISS_SA	 150
#define DALI_CMD_QUERY_DTR	 152
#define DALI_CMD_QUERY_PHY_MIN	 154
#define DALI_CMD_QUERY_LEVEL	 160
#define DALI_CMD_QUERY_MAX		  161
#define DALI_CMD_QUERY_MIN		  162

/*
* ->DALI special commands (144 ... 255)
* (IEC-60929, E.4.3.3.4)
* note that the following codes
* does not match with the commands
* numbers in the paragraph E.4.3.3.4.
* These code are the real command
* representation from binary code.
* e.g.:
* Command 261: 1010 1001 0000 0000 "COMPARE"
* 1010 1011 = 169
*/
#define DALI_CMD_TERMINATE  161
#define DALI_CMD_STORE_DTR  163
#define DALI_CMD_INITIALISE 165
#define DALI_CMD_RANDOMISE  167
#define DALI_CMD_COMPARE   169
#define DALI_CMD_WITHDRAW 171

#define DALI_CMD_SEARCHADDRH  177
#define DALI_CMD_SEARCHADDRM  179
#define DALI_CMD_SEARCHADDRL  181

#define DALI_CMD_PROG_SA  183
#define DALI_CMD_VERIFY_SA  185
#define DALI_CMD_QUERY_SA 187


#endif
