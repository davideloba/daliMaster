#include "daliMaster.h"

//______________________________public_________________________

bool DALIMASTER::begin(const int16_t &myDevice){


	Serial.println(F("i2c master begin.."));

	if(myDevice < 0 || myDevice > 127){
		Serial.println(F("wrong device address!"));
		return false;
	}

	Wire.beginTransmission(myDevice);
	int res = Wire.endTransmission();

	if(!res == 0){
		Serial.println(F("no answer from device!"));
		return false;
	}

	setDevice(myDevice);
	return true;
}


uint8_t DALIMASTER::scanI2cBus(uint8_t & numDev){

	numDev = 0;
	uint8_t i, res = 0;

	for(i = 0; i < 127; i++){

	   Wire.beginTransmission(i);
	   res = Wire.endTransmission();

	   if(res == 0){

		   Serial.print(F("i2c device found at: 0x"));
		   if(i < 16)
			   Serial.print(F("0"));
		   Serial.print(i, HEX);
		   Serial.println();
		   numDev++;
	   }
	}

	return true;
}


bool DALIMASTER::setNewAddr(uint8_t addr){

	if(addr < 1 || addr > 127){
		Serial.println(F("wrong address, not set"));
		return false;
	}

    //negated addr value to check by the chip
    unsigned char data[] = {(unsigned char) addr, (unsigned char) ~(addr)};
    return i2cWrite(this->device, LW14_REG_ADDR, 2, &data[0]);
}


bool DALIMASTER::regRead(uint8_t addr, uint8_t *data){

	uint8_t length = 0;

	switch(addr){

	case LW14_REG_STATUS:
		length = LW14_REG_STATUS_LENGTH;
		break;
	case LW14_REG_CMD:
		length = LW14_REG_CMD_LENGTH;
		break;
	case LW14_REG_SIGNATURE:
		length = LW14_REG_SIGNATURE_LENGTH;
		break;
	default:
		Serial.println(F("wrong register address"));
		return false;
	}

	return lw14Read(addr, length, data);
}


bool DALIMASTER::directCmd(uint8_t addr, uint8_t arc){
   //direct arc power control command (E.4.3.3.1.1)
  switch(arc){

  case 0 ... 255:{
	  return cmdSend(addr, arc);
  }break;

  default:
	Serial.println(F("wrong arc level"));
  }

  return false;
}

bool DALIMASTER::indirectCmd(uint8_t addr, uint8_t arc){
	//indirect arc power control commands (E.4.3.3.1.2)

	switch(arc){

	case 0 ... 31:{
		return cmdSend(addr, arc);
	} break;

	default:
		Serial.println(F("wrong indirect command"));
	}

	return false;
}


bool DALIMASTER::configCmd(uint8_t addr, uint8_t cmd){
	//configuration commands (E.4.3.3.2)

	switch(cmd){

	case 32 ... 128:{
		return cmdSendTwice(addr, cmd);
	} break;

	default:
		Serial.println(F("wrong configuration command"));
	}

	return false;
}

bool DALIMASTER::queryCmd(uint8_t addr, uint8_t cmd){
	//configuration commands (E.4.3.3.3)
	switch(cmd){

	case 144 ... 255:{ //query commands
		return cmdSend(addr, cmd);
	} break;

	default:
		Serial.println(F("wrong query command"));
	}

	return false;
}


bool DALIMASTER::specialCmd(uint8_t cmd1, uint8_t cmd2){
	//special commands (E.4.3.3.4)

	/*
	 * TODO
	 *
	 * Switch case to grab if we need to doubled command
	 *
	 *
	 */
	return cmdSend(cmd1, cmd2);
	//or return cmdSendTwice(cmd_1, cmd_2);
}

uint8_t DALIMASTER::setBroadcastAddress(unsigned char mode){
	return (0xFE | mode);
}


uint8_t DALIMASTER::setGroupAddress(uint8_t group, uint8_t mode){
	return (0x80 | ((group & 16) << 1) | mode);
}


uint8_t DALIMASTER::setShortAddress(uint8_t device, uint8_t mode){
	return (((device & 63) << 1) | mode);
}


//______________________________private______________________________________

void DALIMASTER::setDevice(uint8_t setMe){

	this->device = setMe;
	Serial.print(F("device(0x"));
	if(this->device < 16)
		Serial.print(F("0"));
	Serial.print(this->device, HEX);
	Serial.println(F(") is ready!"));

}


bool DALIMASTER::cmdSendTwice(uint8_t addr, uint8_t command){

	for(uint8_t i = 0; i < TWICE; i++){
		if(!cmdSend(addr, command)) return false;
	}
	return true;
}


bool DALIMASTER::cmdSend(uint8_t addr, uint8_t command){
	return lw14Write(addr, command);
}


bool DALIMASTER::lw14Write(uint8_t data1, uint8_t data2){

	uint8_t data[] = { data1, data2 }; //2 byte commands

    //DEBUG
    Serial.print(F("dali telegram --> byte 1: "));
    Serial.print(data[0]);
    Serial.print(F("("));
    Serial.print(data[0], BIN);
    Serial.print(F("), byte 2: "));
    Serial.print(data[1]);
    Serial.print(F("("));
	Serial.print(data[1], BIN);
	Serial.print(F(")"));
	Serial.println();

    return i2cWrite(this->device, LW14_REG_CMD, 2, &data[0]) ;
}


bool DALIMASTER::lw14Read(uint8_t addr, uint8_t length, uint8_t *data){

 if(length > 6 || length < 1){
	 Serial.println(F("wrong or not handled register length!"));
	 return false;
 }

  return i2cRead(this->device, addr, length, data);
}


bool DALIMASTER::i2cWrite(uint8_t device, uint8_t addr, uint8_t length, uint8_t *data){

	Wire.beginTransmission(device); //select slave
	Wire.write(addr);//move to this register

	for(int i = 0; i < length; i++){
		Wire.write(data[i]); //write 1 byte each time
	}

	switch(Wire.endTransmission()){
		case 0:
			Serial.println(F("success"));
			return true;
			break;
		case 1:
			Serial.println(F("data too long to fit in transmit buffer"));
			break;
		case 2:
			Serial.println(F("received NACK on transmit of address"));
			break;
		case 3:
			Serial.println(F("received NACK on transmit of data"));
			break;
		case 4:
			Serial.println(F("other error"));
			break;
		default:
			Serial.println(F("error not handled"));
		}

	return false;
}


bool DALIMASTER::i2cRead(uint8_t device, uint8_t addr, uint8_t length, uint8_t *data){

	Wire.beginTransmission(device);
	Wire.write(addr); //move to this register
	Wire.endTransmission();

	Wire.requestFrom(device, length);

	if(Wire.available()){
		for(int i = 0; i < length; i++){
			data[i] = Wire.read();

			Serial.println(data[i]); //Debug
		}
	}else{
		Wire.endTransmission();
		return false;
	}

	Wire.endTransmission();
	return true;
}
