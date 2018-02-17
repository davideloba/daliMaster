# serialControl.ino

Here there is a list of commands that you can send to your Arduino™ through serial port to control daliMaster shield.

## Getting started

Load the program *serialControl.ino* under *examples/* folder, open serial monitor and set baudrate to 115000. Write your command in the upper form and press return to send it. Remember that you can send up to 4 arguments at a time. The program will echo the received command and if it is correct it will be execute.
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

#### register reading example
In this example we will ask ballast its phisicaly minimun level, and read the response. First query lamp with short address 4 with DALI_CMD_QUERY_PHY_MIN code (154).
```
**-q -s 4 154**
add
```
Now if we read the (0x00) STATUS REGISTER we will find this response.
```
**-r 0**
add
```
This register will change quickly after the query in this way.
000
000
000
So we have a Valid reply and it is a one byte telegram. So read the (0x01) COMMAND REGISTER to get this telegram.  
```
**-r 1**
add
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
e.g.:
```
**-d -s 6 200**
```
Command ballast with address 6 to 200 arc power level.
e.g.:
```
**-d -b 0**
```
Command all ballast to 0 arc power level.
#### indirect command
**-i [ADDRESS] [COMMAND]**
e.g.:
```
**-i -b 5**
```
Set all ballast arc power levels to the "MAX LEVEL" without fading.
#### configuration command
**-c [ADDRESS] [COMMAND]**
e.g.:
```
**-c -s 9 5**
```
#### query command
**-q [ADDRESS] [COMMAND]**
e.g.:
```
**-q -s 9 5**
```

#### special command
**-x [COMMAND] [COMMAND]** *Please refer to the DALI specification for details on the commands*
