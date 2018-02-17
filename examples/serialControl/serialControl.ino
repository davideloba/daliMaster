/*
 * daliMaster.ino
 *
 *  Created on: 16 Sep 2017
 *      Author: davideloba
 */


#include <daliMaster.h>
#define MAX_CMD 4

DALIMASTER master;

void flush();
void setup();

void loop() {

  if(Serial.available() > 0){

    delay(100); //wait a moment to fill buffer

    String cmd[MAX_CMD];
    for(int i = 0; i < MAX_CMD; i++){   
      String buf = Serial.readStringUntil(' '); //space char   
      buf = buf.substring(0,buf.indexOf('\r'));
      buf = buf.substring(0,buf.indexOf('\n'));
      cmd[i]= buf;
    }
 
    Serial.print(F("received cmd-> "));
    for(int i = 0; i < MAX_CMD; i++){
        Serial.print(cmd[i]);
        Serial.print(",");
    }
    Serial.println();

    bool res = false;

    if(cmd[0].equals("-s")){ //scan i2c bus

      Serial.println(F("scanning i2c bus.."));

      uint8_t devices = 0;
      res = master.scanI2cBus(devices);
      Serial.print(F("i2c devices found: "));
      Serial.println(devices);

    }else if(cmd[0].equals("-a")){ //-a [new i2c address] set new address to lw14

      Serial.print(F("setting "));
      Serial.print(cmd[1]);
      Serial.println(F(" as new i2c address to lw14.."));
      res = master.setNewAddr(cmd[1].toInt());

    }else if(cmd[0].equals("-r")){ //-r [reg address] read lw14 register

      Serial.println(F("reading lw14 register.."));

      uint8_t data[LW14_REG_SIGNATURE_LENGTH]; //max reg length
      res = master.regRead(cmd[1].toInt(), data);
      master.printReg(cmd[1].toInt(), data);

    }else{  //define and send DALI forward telegram

      uint8_t daliMode, daliAddr, daliCmd = 0;
      daliMode = cmd[0].equals("-d")? LW14_MODE_DACP : LW14_MODE_CMD;

      if(!cmd[0].equals("-x")){ //do this just for non-special(x) cmd

        if(cmd[1].equals("-s")){

          if(cmd[2].toInt() < 0 || cmd[2].toInt() > 63){
            Serial.println(F("wrong short address"));
            return;
          }

          daliAddr = master.setShortAddress(cmd[2].toInt(),daliMode);
          daliCmd = cmd[3].toInt();

        }else if(cmd[1].equals("-b")){

          daliAddr = master.setBroadcastAddress(daliMode);
          daliCmd = cmd[2].toInt();

        }else if(cmd[1].equals("-g")){

          if(cmd[2].toInt() < 0 || cmd[2].toInt() > 15){
            Serial.println(F("wrong group address"));
            return;
          }

          daliAddr = master.setGroupAddress(cmd[2].toInt(),daliMode);
          daliCmd = cmd[3].toInt();

        }else{
          Serial.println(F("wrong recipient type"));
          return;
        }
      }

      if(cmd[0].equals("-d")){ //direct arc power command

        Serial.println(F("direct arc power command"));
        res = master.directCmd(daliAddr, daliCmd);

      }else if(cmd[0].equals("-i")){ //indirect arc power command
        
        Serial.println(F("indirect arc power command"));
        res = master.indirectCmd(daliAddr, daliCmd);

      }else if(cmd[0].equals("-c")){ //configuration command
        
        Serial.println(F("configuration command"));
        res = master.configCmd(daliAddr, daliCmd);

      }else if(cmd[0].equals("-q")){ //query command
        
        Serial.println(F("query command"));
        res = master.queryCmd(daliAddr, daliCmd);
        if(res)
          Serial.println(F("(now you should read Command reg(0x01) to see the response)"));

      }else if(cmd[0].equals("-x")){ //special command
        
        Serial.println(F("special command"));
        res = master.specialCmd(cmd[1].toInt(), cmd[2].toInt()); //use original cmd

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
  Serial.println("daliMaster start..");

  master.begin(23);
}
