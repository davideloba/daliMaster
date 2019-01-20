# daliMaster

This is an Arduino™ library to control your DALI lamps with the brand new [daliMaster](https://www.ebay.it/itm/DALI-Master-shield-for-Arduino-UNO/253974008596?hash=item3b2207d314:g:yMAAAOSwAbxb6T0g:rk:1:pf:0) shield, with buit-in DALI bus power supply system. B:boom::boom:m!

## Description

### What is DALI?

DALI (Digital Addressable Lighting Interface) is a powerful protocol to control lighting. Through DALI you can dimmer your led lamps, ask them status, recall a predefined scenario and so on. If you want more information about DALI you can find many useful links to the bottom of this page.

### Can I use DALI with my Arduino™?

Well, the answer is YES.

### How?

With [daliMaster](https://www.ebay.it/itm/DALI-Master-shield-for-Arduino-UNO/253974008596?hash=item3b2207d314:g:yMAAAOSwAbxb6T0g:rk:1:pf:0) shield! As the name suggests, that shield transforms your Arduino™ in a DALI master, acting as a bridge between I2C interface and DALI bus. Let's make an example to explain how it works.

## Getting Started

* Fit [daliMaster](https://www.ebay.it/itm/DALI-Master-shield-for-Arduino-UNO/253974008596?hash=item3b2207d314:g:yMAAAOSwAbxb6T0g:rk:1:pf:0) shield on your Arduino™

* Make connections (you can find an example [here](https://www.ebay.it/itm/DALI-Master-shield-for-Arduino-UNO/253974008596?hash=item3b2207d314:g:yMAAAOSwAbxb6T0g:rk:1:pf:0))
  * Connect your lamps to their ballasts
  * Connect your ballast to mains..be careful!
  * Connect your ballasts and [daliMaster](https://www.ebay.it/itm/DALI-Master-shield-for-Arduino-UNO/253974008596?hash=item3b2207d314:g:yMAAAOSwAbxb6T0g:rk:1:pf:0) shield to DALI bus
  * Connect your 24V DC power supply to mains and to [daliMaster](https://www.ebay.it/itm/DALI-Master-shield-for-Arduino-UNO/253974008596?hash=item3b2207d314:g:yMAAAOSwAbxb6T0g:rk:1:pf:0)..again, be careful!

* If I'm right, now you should have all of lamps on. Let's turn them off.

* Connect Arduino™ to your computer.

* Download this library and load **daliMaster/examples/serialControl.ino**

* Now open serial monitor, select 115000 as baudrate and you should see:
```
daliMaster start..
i2c master begin..
device(0x23) is ready!
```
* Well, write and send this command:
```
-d -b 0
```
* If everything went well your lamps now are off. But we don't like darkness, so let's switch them on to the minimum:
```
-d -b 1
```
* Cool! Let's push them to maximum:
```
-d -b 254
```
Easy, isn't it? Now you can modulate all lamps from 0 up to 254 with those simple commands. :thumbsup:

## Next

See more informations about serial commands [here](/examples/serialControl/README.MD). See other examples to play with your lamps (I suggest to try [Pulse.ino](/examples/pulse)). See also the following links to know more about DALI and LW14, the chip from which the [daliMaster](https://www.ebay.it/itm/DALI-Master-shield-for-Arduino-UNO/253974008596?hash=item3b2207d314:g:yMAAAOSwAbxb6T0g:rk:1:pf:0) is powered by.

## Useful links

### LW14
* [LW14 datasheet](https://www.codemercs.com/downloads/ledwarrior/LW14_Datasheet.pdf)

### DALI
* [main commands](http://www.tanzolab.it/www/CM3-HOME_test/dali_commands.pdf)
* DALI international standard (English/French) [60929 © IEC:2006](http://jnhb.fszjzx.com/upload/biaozhun/pdf/IEC60929Y2006.PDF)

## Built With

* [Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/packages/eclipse-ide-cc-developers/lunar)
* [Arduino IDE](https://www.arduino.cc/en/main/software)

## Versioning

* v.1 First release 21/01/2018

## Credits

* **Code Mercenaries** - *Original Raspberry Pi library for LW14 Modules* - [LED-Warrior14](https://www.codemercs.com/en/software)

see [credits.md](credits.md) file for details.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
