#ifndef ZIGBEE_MODE_ED
#error "Zigbee end device mode is not selected in Tools->Zigbee mode"
#endif

#include <Zigbee.h>
#include "ZigbeeWrapper.h"

#define OCCUPANCY_SENSOR_ENDPOINT_NUMBER    3
ZigbeeOccupancySensor zbOccupancySensor = ZigbeeOccupancySensor(OCCUPANCY_SENSOR_ENDPOINT_NUMBER);

void ConfigureZigbee()
{
  zbOccupancySensor.setManufacturerAndModel("Fox-Nest Inc", "foxnest.presence-detector");

  Zigbee.addEndpoint(&zbOccupancySensor);
  Serial.println("Starting Zigbee...");
  // When all EPs are registered, start Zigbee in End Device mode
  if (!Zigbee.begin()) {
    Serial.println("Zigbee failed to start!");
    Serial.println("Rebooting...");
    ESP.restart();
  } else {
    Serial.println("Zigbee started successfully!");
  }
  Serial.println("Connecting to network");
  while (!Zigbee.connected()) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
}
