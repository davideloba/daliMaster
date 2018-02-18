# serialControl.ino

Here there is a list of commands that you can send to your Arduino™ through serial port to control daliMaster shield.

## Getting started

Load the program *serialControl.ino* under *examples* folder, open serial monitor and set baudrate to 115000. Write your command in the upper form and press return to send it. Remember that you can send up to 4 arguments at a time. The program will echo the received command and if it is correct it will be execute.
### Hello world
```
daliMaster start..
i2c master begin..
device(0x23) is ready!
```
### I2C bus scan

**-s**  
Scan I2C bus and report if any I2C devices has been found with their address. Default daliMaster address is 35 (0x23).
```
-s
received cmd-> -s,,,,
scanning i2c bus..
i2c device found at: 0x23 (35)
i2c devices found: 1
done.
```
### Set new I2C address
**-a [new address]**  
Set another I2C address to the daliMaster LW14 chip. Values of 128 and more are not accepted.
```
-a 24
received cmd-> -a,24,,,
setting 24 as new i2c address to lw14..
success
done.
```
### Read LW14 register
**-r [register address]**

Read LW14 register and echo result.
* (0x00) STATUS REGISTER  
The status register is one byte that contains the bus status and command status flags:  
7 - Bus Error Status *(0 = Bus OK, 1 = Bus fault)*  
6 - Busy *(0 = ready, 1 = busy)*  
5 - Overrun  
4 - Frame Error  
3 - Valid Reply  
2 - Reply Timeframe (*< 22 Te since last command 1 or 2 Byte telegram received*)  
1 - Two byte telegram received
0 - One byte telegram received
* (0x01) COMMAND REGISTER  
The command register has two bytes which directly represent the DALI command. Please refer to the DALI specification for details on the commands.
* (0xFE) SIGNATURE REGISTER
The signature register can be used to identify LW14 and get the revision information for the chips firmware.
See [LW14 datasheet](http://shop.codemercs.com/media/files_public/okutobbwyxn/LW14_Datasheet.pdf) for more details.

#### Register reading example
In this example we will ask ballast its phisicaly minimun level and read the response.
First do a dummy reading to free previous messagges on 0x01 LW14 register. Do not mind output.
```
-r 1
```
Query lamp with short address 8 with DALI_CMD_QUERY_PHY_MIN code (154).
```
-q -s 8 154
received cmd-> -q,-s,8,154,
query command
dali telegram --> byte 1: 17(00010001), byte 2: 154(10011010)
success
(now you should read Command reg(0x01) to see the response)
done.
```
Now if we read the (0x00) STATUS REGISTER we will find that a reply is available.
```
-r 0
received cmd-> -r,0,,,
reading lw14 register..
(0x00) Status reg: 41 ->bits
  code.  BUS BUSY OVER ERR	REPLY	TIME	2TEL	1TEL
	value  0   0	  0    0    1     0     0     1
done.
```
This register will change quickly after the query in this way.
```
code.	BUS BUSY  OVER	ERR	REPLY	TIME	2TEL	1TEL
value	0  1	0 0 0 0 0 0
```
Busy = '1' indicates that the last command has not yet been transmitted. Any new command sent to register 1 will be ignored until the last command has been transmitted and the busy bit is cleared.
```
code.	BUS BUSY  OVER	ERR	REPLY	TIME	2TEL	1TEL
value	0  0  0 0 0 1 0 0
```
Time = 1 indicates that the time frame for a reply from the last addressed device has not yet timed out and is reset to zero after 22 Te (see DALI specification) or on bus activity.
```
code. BUS BUSY  OVER  ERR REPLY TIME  2TEL  1TEL
value 0 0	0 0 1 0 0 1
```
Valid Reply = 1 if a telegram has been received within 22 Te (see DALI specification) of sending a command. 1Tel = 1 means that 1 byte telegram has been received. The bit is reset on reading register 0x01.


So we have a Valid reply and it is a one byte telegram. So read the (0x01) COMMAND REGISTER to get this telegram.  
```
-r 1
received cmd-> -r,1,,,
reading lw14 register..
(0x01) Command reg: 170..0..done.
```
So, ballast physical minimum is 170. Notice that even if DALI permits 254 levels, ballast cannot dim light under this value.

### Address
There are 3 types of addresses:
* **-s [NUM]** short addresses: as DALI specification, each ballast can be reached with a single address from 0 up to 63. When this short address is not already set, ballast reacts only to broadcast commands. To assign this address see the example.
* **-g [NUM]** group addresses: from 0 up to 15. *Please refer to the DALI specification for details on the commands*
* **-b** broadcast: commands sent with a broadcast address will reach every ballast.

### DALI forward telegram
#### direct ARC command
**-d [ADDRESS] [LEVEL]**
```
-d -s 8 200
```
Command ballast with address 8 to 200 arc power level.
```
-d -b 0
```
Command all ballast to switch to 0 (off).
#### indirect command
**-i [ADDRESS] [COMMAND]**
```
-i -b 5
```
Set all ballast arc power levels to the "MAX LEVEL" without fading.
#### configuration command
**-c [ADDRESS] [COMMAND]**
```
-c -s 8 128
```
Tell ballast 8 to store DTR as its short address.
#### query command
**-q [ADDRESS] [COMMAND]**
```
-q -s 8 160
```
Ballast will response with actual arc power level. To read the response, see register reading specifications.
#### special command
**-x [COMMAND] [COMMAND]** *Please refer to the DALI specification for details on the commands*
