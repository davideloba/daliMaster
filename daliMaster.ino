/*
 * daliMaster.ino
 *
 *  Created on: 16 Sep 2017
 *      Author: davideloba
 */
#include <Arduino.h>
#include "src/daliMaster.h"

#define MAX_SIZE 20 //without NULL char
#define MAX_CMD 4

DALIMASTER master;

void flush();
void setup();

void loop() {

	if(Serial.available() > 0){
    
		delay(100); //wait a moment to fill buffer

		char buf[MAX_SIZE+1];
		uint8_t endBuf = Serial.readBytes(buf, MAX_SIZE);
		flush();

		buf[endBuf] = 0;

		char* cmd[MAX_CMD+1];
		cmd[0] = strtok(buf, " "); //space separator
		cmd[1] = strtok(NULL, " ");
		cmd[2] = strtok(NULL, " ");
		cmd[3] = strtok(NULL, " ");

		//DEBUG
		Serial.print(F("received cmd--> "));
		for(int i = 0; i < MAX_CMD; i++){
			Serial.print("..");
			Serial.print(cmd[i]);
		}
		Serial.println();

		bool res = false;

		if(!strcmp(cmd[0],"-s")){ //scan i2c bus

			Serial.println(F("scanning i2c bus.."));

			uint8_t devices = 0;
			res = master.scanI2cBus(devices);
			Serial.print(F("i2c devices found: "));
			Serial.println(devices);

		}else if(!strcmp(cmd[0],"-a")){ //-a [new i2c address] set new address to lw14

			Serial.println(F("setting new i2c address to lw14.."));
			res = master.setNewAddr(atoi(cmd[1]));

		}else if(!strcmp(cmd[0],"-r")){ //-r [reg address] read lw14 register

			Serial.println(F("reading lw14 register.."));

			uint8_t data[LW14_REG_SIGNATURE_LENGTH]; //max reg length
			memset(data,0,sizeof(data));

			res = master.regRead(atoi(cmd[1]), data);

			if(res){

				Serial.print(F("(0x"));
				if(res < 16) Serial.print(F("0"));
				Serial.print(atoi(cmd[1]), HEX);
				Serial.print(F(")"));

				switch(atoi(cmd[1])){

					case LW14_REG_STATUS:

						Serial.print(F(" Status reg"));
						Serial.print(F(": "));
						Serial.print(data[0]);
						Serial.println(F(" --> "));
						Serial.println(F("bit no.: 76543210"));
           Serial.print(F("         "));
						Serial.println(data[0], BIN);

						break;

					case LW14_REG_CMD:

						Serial.print(F(" Command reg: "));
						Serial.print(data[0]);
						Serial.print(F(".."));
						Serial.println(data[1]);

						break;

					case LW14_REG_SIGNATURE:

						Serial.print(F(" Signature reg: "));
						for(int i = 0; i < LW14_REG_SIGNATURE_LENGTH; i++){
							Serial.print(data[i]);
							Serial.print(F(".."));
						}
						Serial.println();

						break;
				}
			}

		}else{  //define and send DALI forward telegram

			uint8_t daliMode, daliAddr, daliCmd = 0;
			daliMode = !strcmp(cmd[0],"-d")? LW14_MODE_DACP : LW14_MODE_CMD;

			if(strcmp(cmd[0],"-x")){ //do this just for non-special(x) cmd

				if(!strcmp(cmd[1],"-s")){

					if(atoi(cmd[2]) < 0 || atoi(cmd[2]) > 63){
						Serial.println(F("wrong short address"));
						return;
					}

					daliAddr = master.setShortAddress(atoi(cmd[2]),daliMode);
					daliCmd = atoi(cmd[3]);

				}else if(!strcmp(cmd[1],"-b")){

					daliAddr = master.setBroadcastAddress(daliMode);
					daliCmd = atoi(cmd[2]);

				}else if(!strcmp(cmd[1],"-g")){

					if(atoi(cmd[2]) < 0 || atoi(cmd[2]) > 15){
						Serial.println(F("wrong group address"));
						return;
					}

					daliAddr = master.setGroupAddress(atoi(cmd[2]),daliMode);
					daliCmd = atoi(cmd[3]);

				}else{

					Serial.println(F("wrong recipient type"));
					return;
				}
			}

			if(!strcmp(cmd[0],"-d")){ //direct arc power command

				Serial.println(F("direct arc power command"));
				res = master.directCmd(daliAddr, daliCmd);

			}else if(!strcmp(cmd[0],"-i")){ //indirect arc power command
				Serial.println(F("indirect arc power command"));
				res = master.indirectCmd(daliAddr, daliCmd);

			}else if(!strcmp(cmd[0],"-c")){ //configuration command
				Serial.println(F("configuration command"));
				res = master.configCmd(daliAddr, daliCmd);

			}else if(!strcmp(cmd[0],"-q")){//query command
				Serial.println(F("query command"));
				res = master.queryCmd(daliAddr, daliCmd);
				if(res)
					Serial.println(F("(now you should read Command reg(0x01) to see the response)"));

			}else if(!strcmp(cmd[0],"-q")){//special command
				Serial.println(F("special command"));
				res = master.specialCmd(atoi(cmd[1]), atoi(cmd[2])); //use original cmd

			}else{
				Serial.println(F("wrong command"));
				return;
			}
		}


		if(res){
			Serial.println(F("done."));
		}else{
			Serial.println(F("error."));
		}

	}


} //end of loop

//---functions

void flush(){
	while(Serial.available()>0) Serial.read();
}


void setup() {

	Wire.begin();
	Serial.begin(BAUDRATE);
	Serial.setTimeout(1); //useful
		delay(1000);
	Serial.println("Start..");

	master.begin(LW14_I2CADR);
}



