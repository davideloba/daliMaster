/*
 * daliMaster.ino
 *
 *  Created on: 16 Sep 2017
 *      Author: davideloba
 */

#include <daliMaster.h>

uint8_t data[LW14_REG_CMD_LENGTH];
uint8_t daliAddr;

DALIMASTER master;

void flush();
void setup();

void loop() {

	if(Serial.available() > 0){

		delay(100); //wait a moment to fill buffer

		String buf = Serial.readStringUntil('\n');
		flush(); //needed

		uint16_t daliSa = buf.toInt();

		bool res = false;

		switch (daliSa) {

      case 0 ... 63:{

				Serial.print(F("This command will assign \'"));
				Serial.print(daliSa);
				Serial.print(F("\' as new address with a broadcasted command."));
				Serial.println(F("Be sure that just one ballast will receive it. Continue? (digit \"Y\" for YES)"));

				while(true){ //wait for Y

					if(Serial.available() > 0){

						delay(100); //wait a moment to fill buffer

						String buf = Serial.readStringUntil('\n');

						if(buf.equals("Y") | buf.equals("y") ){

							/*
							60929 © IEC:2006
							Only one ballast connected separately to the control unit for a simplified addressing method:
							Send first the new short address (0AAA AAA1) by command 257 "DATA TRANSFER REGISTER (DTR)",
							verify the content of the DTR and send command 128 "STORE DTR AS SHORT ADDRESS" two times.
							*/

							daliAddr = master.setBroadcastAddress(LW14_MODE_CMD);

							//store address in DTR.
							if(!master.specialCmd(DALI_CMD_DTR, newAddr))
								return;

							//read it back and check it.
							if(!master.queryCmd(daliAddr, DALI_CMD_QUERY_DTR))
								return;

							if(!master.regRead(LW14_REG_CMD, data))
								return;

							if(data[0] != newAddr)
								return;

							//store DTR as new address.
							if(!master.configCmd(daliAddr, DALI_CMD_STORE_DTR_SA))
								return;

							//ask lamp if missing short address or not.
							if(!master.queryCmd(daliAddr, DALI_CMD_QUERY_BALLAST))
								return;

							if(!master.regRead(LW14_REG_CMD, data))
								return;

							if(data[0] != DALI_YES)
								return;

							res = true;

						}

					}
				}

      }break;

			default:

				Serial.print(F("wrong DALI short address. "));

		} //end of switch

		if(res){
			Serial.println(F("New address assigned and verified. Done."));
		}else{
			Serial.println(F("Error."));
		}

		flush();
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

	if(!master.begin(LW14_I2CADR)){
		exit(1);
	}

	delay(2000);
	Serial.println("Digit address to set (0-63):");
}
