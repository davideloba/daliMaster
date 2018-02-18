#include <daliMaster.h>
DALIMASTER master;

uint8_t myData[LW14_REG_CMD_LENGTH];
uint32_t previous = 0;

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
				Serial.println(F("\' as new address with a broadcasted command."));
				Serial.println(F("Be sure that just one ballast will receive it. Continue? (digit \"Y\" for YES)"));

				while(true){ //wait for Y

					if(Serial.available() > 0){

						delay(100); //wait a moment to fill buffer

						String buf = Serial.readStringUntil('\n');

						if(buf.indexOf('Y') != -1 | buf.indexOf('y') != -1){

							/*
							60929 © IEC:2006
							Only one ballast connected separately to the control unit for a simplified addressing method:
							Send first the new short address (0AAA AAA1) by command 257 "DATA TRANSFER REGISTER (DTR)",
							verify the content of the DTR and send command 128 "STORE DTR AS SHORT ADDRESS" two times.
							*/

              uint8_t dtr = master.setShortAddress(daliSa, LW14_MODE_CMD); //DTR structure 0AAA AAA1
              uint8_t broadcastAddr = master.setBroadcastAddress(LW14_MODE_CMD);

              Serial.println(F("\r\n---> Store new address into DTR <---")); //DEBUG
              
							if(!master.regClean() | !master.specialCmd(DALI_CMD_STORE_DTR, dtr)){
                Serial.println(F("unable to set DTR!"));
                break;
              }
              
              if(!master.waitForIdle(DALI_BUS_TIMEOUT)){
								Serial.println(F("idle timeout!")); //DEBUG
								break;
							}

              Serial.println(F("\r\n---> Read DTR back and check it again.. <---")); //DEBUG

							if(!master.regClean() | !master.queryCmd(broadcastAddr, DALI_CMD_QUERY_DTR)){
                Serial.println(F("unable to read DTR back!"));
                break;
              }

              if(!master.waitForTel1(DALI_BUS_TIMEOUT)){
                Serial.println(F("telegram timeout!")); //DEBUG
                break;
              }
              
							if(!master.regRead(LW14_REG_CMD, myData) | myData[0] != dtr){
								Serial.println(F("DTR does not match!")); //DEBUG
								break;
							}

							Serial.println(F("\r\n--> DTR match! Now save as new address <--")); //DEBUG

							if(!master.configCmd(broadcastAddr, DALI_CMD_STORE_DTR_SA))
								break;

							if(!master.waitForIdle(DALI_BUS_TIMEOUT)){
								Serial.println(F("idle timeout!")); //DEBUG
								break;
							}

              Serial.println(F("\r\n--> Ask if there is a ballast with the given address that is able to communicate <--")); //DEBUG

							if(!master.regClean() | !master.queryCmd(master.setShortAddress(daliSa, LW14_MODE_CMD), DALI_CMD_QUERY_BALLAST))
								break;

							if(!master.waitForTel1(DALI_BUS_TIMEOUT)){
								Serial.println(F("telegram timeout!")); //DEBUG
								break;
							}

							if(!master.regRead(LW14_REG_CMD, myData) | myData[0] != DALI_YES)
								break;

              Serial.println(F("\r\n--> Well, now make it flash! <--")); //DEBUG

              master.indirectCmd(master.setShortAddress(daliSa, LW14_MODE_CMD), DALI_CMD_OFF);
              master.waitForBus(DALI_BUS_TIMEOUT);
                delay(200);
              master.indirectCmd(master.setShortAddress(daliSa, LW14_MODE_CMD), DALI_CMD_MAX_LEVEL);
							res = true;
              
             // break; //exit while(true);
             
						}
						
						/*else{
              break;
						}*/

            break;

					} //end of if Serial is available
				} //end of while(true)

      } break;

			default:

				Serial.println(F("wrong DALI short address. "));

		} //end of switch

		if(res){
			Serial.println(F("\r\n--> New address assigned and verified. Done. <--"));
      Serial.println();
		}else{
			Serial.println(F("\r\n--> Error. <--"));
      Serial.println();
		}

    Serial.println(F("Digit address to set (0-63) to repeate the procedure."));
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

	if(!master.begin(LW14_DEF_ADDR))
		exit(1);

  Serial.println("Digit address to set (0-63):");
}
