#include <daliMaster.h>
DALIMASTER master;

uint8_t daliAddr = 0;

void setup() {

  Wire.begin();
  Serial.begin(BAUDRATE);
  Serial.setTimeout(1); //useful
    delay(1000);
  Serial.println("Start..");

  master.begin(LW14_I2CADR);
  daliAddr = master.setBroadcastAddress(LW14_MODE_DACP);
}

void loop() {

  for(int i = 254; i > 150; i--){

    if(!master.waitForBus(DALI_BUS_TIMEOUT) | !master.directCmd(daliAddr, i))
      Serial.println(F("error."));
  }

  for(int i = 150; i < 255; i++){

   if(!master.waitForBus(DALI_BUS_TIMEOUT) | !master.directCmd(daliAddr, i))
      Serial.println(F("error."));
  }
}
