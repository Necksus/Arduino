/*
Tools -> Upload speed : 115200
Tools -> USB CDC on boot :	Enabled (pour avoir debug port série)
Tools -> Zigbee mode:		Zigbee ED (end device)
Tools -> Partition Scheme:	Zigbee 4MB with spiffs
Tools -> Core Debug Level:	Debug (or none)

Sample Expressif
https://github.com/espressif/arduino-esp32/blob/3.0.7/libraries/Zigbee/examples/Zigbee_On_Off_Light/Zigbee_On_Off_Light.ino

https://docs.espressif.com/projects/esptool/en/latest/esp32h2/advanced-topics/boot-mode-selection.html
To manually reset a development board, hold down the Boot button (GPIO9) and press the EN button (EN (CHIP_PU))

Sample
https://fixtse.com/blog/mini-mmwave-sensor

*/

#include "LD2450Wrapper.h"
#include "ZigbeeWrapper.h"

ZigbeeWrapper m_zigbee;
LD2450Wrapper m_ld2450;

void setup() {
  // put your setup code here, to run once:
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("ESP32H2 starting up.....");
#endif

  m_zigbee.Setup();
  m_ld2450.Setup();
}


void loop()
{
  if (m_ld2450.HasUpdate())
  {
    bool occupancy = false;

    for (uint8_t targetId = 0; targetId < 3; targetId++)
    {
      auto target = m_ld2450.GetTarget(targetId);
      m_zigbee.UpdateTarget(targetId, target.isValid(), target.x, target.y);
      if (target.isValid())
      {
        occupancy = true;
      }
    }
    m_zigbee.UpdateOccupancy(occupancy);
    delay(1000);
  }
}
