# serialControl.ino wiki

Here there is a list of commands that you can send to your Arduino™ through serial port to control daliMaster shield.

## Getting started

Load the program *serialControl.ino* under *examples/* folder, open serial monitor and set baudrate to 115000. Write your command in the upper form and press return to send it. Remember that you can send up to 4 arguments at a time. The program will echo the received command and if it is correct it will be execute.

### I2C bus scan
**-s**
scan I2C bus and report if any I2C devices has been found and their address. Default daliMaster address is 35 (0x23).
```
-s
TODO: insert respose
```
### I2C bus scan
**-a [new address]**
Set by software another I2C address to the daliMaster I2C chip. Values of 128 and more are not accepted.
```
-a 24
TODO:
```
### Read LW14 register
**-r [register address]**
*0    [STATUS]
1   [COMMAND]
240   [SIGNATURE]*

Read LW14 register and echo result.
* STATUS REGISTER  
The status register is one byte that contains the bus status and command status flags:
7 - Bus Error Status *(0 = Bus OK, 1 = Bus fault)*  
6 - Busy *(0 = ready, 1 = busy)*  
5 - Overrun  
4 - Frame Error  
3 - Valid Reply  
2 - Reply Timeframe (*< 22 Te since last command 1 - 2 Byte telegram received*)  
0 - One byte telegram received  
See [LW14 datasheet](http://shop.codemercs.com/media/files_public/okutobbwyxn/LW14_Datasheet.pdf) for more details.  
* COMMAND REGISTER  
The command register has two bytes which directly represent the DALI command. The first byte is the address byte, the second the command byte. Please refer to the DALI specification for details on the commands.
* SIGNATURE REGISTER  
See [LW14 datasheet](http://shop.codemercs.com/media/files_public/okutobbwyxn/LW14_Datasheet.pdf).

### send DALI forward telegram
**[ARG0] [ARG1] [ARG2] [ARG3]**

* [ARG0]
* -d *direct ARC command*
* -i *indirect command*
* -c *configuration command*
* -q *query command*

  * [ARG1]
  * -b *broadcast address*

    * [ARG2] *dali command*

  * [ARG1]  
  * -g *group address follow*
  * -s *slave short address follow*

    * [ARG3]
    * [SLAVE ADDRESS] *dali slave short address, from 0 up to 63*
    * [GROUP ADDRESS] *dali group address, from 0 up to 15*

      * [ARG4] *dali command*

* [ARG0]
* -x *special command*
  * [ARG2] [ARG3] *Please refer to the DALI specification for details on the commands*
