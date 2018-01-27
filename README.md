# daliMaster
========

This is an Arduino™ library to control your DALI lamps with brand new [daliMaster](www.ebay.d) Arduino™ shield, with buit-in DALI bus supply system. Boom!

## Description

### What is DALI?

DALI (Digital Addressable Lighting Interface) is a powerful protocol to control lighting. Through DALI you can dimmer your led lamps, ask them status, recall a predefined scenario and so on. Of course, the ballast that powers your lamp must be DALI compatible.
You want more information about DALI, don't you? I thinks this is not the right place for that but I'm gonna leave you some useful links to the bottom of this page.

### Can I use DALI with my Arduino™?

Well, the answer is YES.

### How?

[daliMaster](www.ebay.d) is an shield that can transform your Arduino™ in a DALI master. [daliMaster](www.ebay.d) acts as a bridge between Arduino™ I2C interface and DALI bus. Let's make an example to examplain how it works.

## Getting Started

* Fit [daliMaster](www.ebay.d) on your Arduino™

* Make connections (You can find an example schema [here](www.ebay.d))
** Connect your lamps to their ballasts
** Connect your ballast to mains..be careful!
** Connect your ballasts and [daliMaster](www.ebay.d) to DALI bus
** Connect your AC/DC 24 V power supply to mains and to [daliMaster](www.ebay.d)..be careful! Remember that you need to supply [daliMaster](www.ebay.d) and the shield will do the rest in order to power DALI bus.

* Connect Arduino™ to your computer.

* If I'm right, now you should have all of lamps On. Let's turn them off.

* Download this library and load /examples/

 * Open serial monitor, select 115000 as baudrate and write:
```
-b 0 0
```
If everything went well your lamps now are off. But we don't like darkness, so let's switch them on to the minimun. Write:
```
-b 0 1
```

### Prerequisites

What things you need to install the software and how to install them

```
Give examples
```

### Installing

A step by step series of examples that tell you have to get a development env running

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### Useful links

#### DALI

#### LW14

```
Give an example
```

## Built With

* [Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/packages/eclipse-ide-cc-developers/lunar)
* [Arduino IDE](https://www.arduino.cc/en/main/software)

## Versioning

* v.1 First release 21/01/2018

## Credits

* **Code Mercenaries** - *Original Raspberry Pi library for LW14 Modules* - [LED-Warrior14](https://www.codemercs.com/en/33-led-warrior/270-led-warrior14-2)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
