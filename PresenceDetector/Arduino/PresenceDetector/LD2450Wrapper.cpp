
#include <LD2450.hpp>   // https://github.com/Fiooodooor/HLK-LD245X/tree/main
#include <HardwareSerial.h>

#include "LD2450Wrapper.h"

using namespace esphome::ld245x;
HardwareSerial ld2450Serial(1);
LD2450 ld2450;

void ConfigureLD2450()
{
  Serial.begin(115200);
  ld2450Serial.begin(LD2450_SERIAL_SPEED, SERIAL_8N1, RX_PIN, TX_PIN);
  ld2450Serial.setTimeout(1000);

  Serial.println("LD2450, HardwareSerial(1) waiting for sensor data...\n");
  ld2450.begin(ld2450Serial, true);  

  ld2450.beginConfigurationSession();
  ld2450.setMultiTargetTracking();
  ld2450.queryTargetTrackingMode();
  ld2450.queryFirmwareVersion();
  ld2450.queryMacAddress();
  ld2450.queryZoneFilter();
  bool configOk = ld2450.endConfigurationSession();
  if (configOk)
  {
    Serial.print("Sensor name: ");
    Serial.println(ld2450.getNameString());

    Serial.print("Zone filter: ");
    Serial.println(ld2450.getZoneFilter());

    Serial.print("Firmware value: ");
    Serial.println(ld2450.getFirmwareString());

    Serial.print("MacAddress value: ");
    Serial.println(ld2450.getMacAddressString());
  }
  else
  {
    Serial.println("Cannot configure LD2450");
  }
}


void ProcessLD2450()
{
  if (ld2450.update())
  {
    auto target1 = ld2450.getTarget(0);
    auto target2 = ld2450.getTarget(1);
    auto target3 = ld2450.getTarget(2);

    char buffer[80];
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d", 
      target1.valid,
      target1.x,
      target1.y,
      target1.v,
      target1.d,
      target1.res,

      target2.valid,
      target2.x,
      target2.y,
      target2.v,
      target2.d,
      target2.res,

      target3.valid,      
      target3.x,
      target3.y,
      target3.v,
      target3.d,
      target3.res);
    
//      Serial.println(buffer);

    
    //Serial.print(ld2450.getLastTargetMessage());
    
  }
}