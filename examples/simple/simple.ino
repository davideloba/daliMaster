#define DALISA 8
#define LEVEL 205

#include <daliMaster.h>

DALIMASTER master;

void setup() {

  Wire.begin();
  Serial.begin(BAUDRATE);
  Serial.setTimeout(1); //useful
    delay(1000);
  Serial.println("Start..");

  if(!master.begin(LW14_DEF_ADDR))
    errorHandler(1);
    
  Serial.print(F("Set lamp "));
  Serial.print(DALISA);
  Serial.print(F(" to "));
  Serial.print(LEVEL);
  Serial.println();

  uint8_t daliAddr = 0;
  daliAddr = master.setShortAddress(DALISA, LW14_MODE_DACP);
  
  if(!master.waitForBus(DALI_BUS_TIMEOUT) | !master.directCmd(daliAddr, LEVEL))
    errorHandler(2);

  Serial.print(F("Now ask lamp level"));
  Serial.println();

  daliAddr = master.setShortAddress(DALISA, LW14_MODE_CMD);

  if(!master.regClean())
    errorHandler(4);
  
  if(!master.waitForBus(DALI_BUS_TIMEOUT) | !master.queryCmd(daliAddr, DALI_CMD_QUERY_LEVEL))
    errorHandler(5);

  uint8_t myData[2];

  if(!master.waitForTel1(DALI_BUS_TIMEOUT) | !master.regRead(LW14_REG_CMD, myData))
    errorHandler(6);

  Serial.print(F("Actual lamp level is "));
  Serial.print(myData[0]);
  Serial.println(F(". End."));
  
}

void loop(){
  //do nothing  
}

void errorHandler(uint8_t code){
  Serial.println(F("error "));
  Serial.println(code);
  Serial.println(F("end."));
  exit(1);
}

