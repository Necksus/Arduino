#ifndef ZIGBEE_MODE_ED
#error "Zigbee end device mode is not selected in Tools->Zigbee mode"
#endif

#include "ZigbeeWrapper.h"

#define ZIGBEE_MANUFACTURER     "Fox-Nest Inc"
#define ZIGBEE_MODEL            "foxnest.presence-detector"
#define MAX_DISTANCE_IN_MM      10000

#define TARGET_ENDPOINT_NUMBER   3


ZigbeeTarget::ZigbeeTarget(uint8_t number)
  : m_IsValid(TARGET_ENDPOINT_NUMBER * (number-1) + 2)
  , m_X(TARGET_ENDPOINT_NUMBER * (number-1) + 3)
  , m_Y(TARGET_ENDPOINT_NUMBER * (number-1) + 4)
{
  m_number = number;
}

void ZigbeeTarget::Setup()
{
  char  buffer[120];

  sprintf(buffer, "Target %d is valid", m_number);
  m_IsValid.addBinaryInput();
  m_IsValid.setBinaryInputApplication(BINARY_INPUT_APPLICATION_TYPE_HVAC_OCCUPANCY);
  m_IsValid.setBinaryInputDescription(buffer);
  Zigbee.addEndpoint(&m_IsValid);

  sprintf(buffer, "Target %d X (millimeter)", m_number);
  m_X.addAnalogInput();
  m_X.setAnalogInputDescription(buffer);
  m_X.setAnalogInputMinMax(-MAX_DISTANCE_IN_MM, MAX_DISTANCE_IN_MM);
  Zigbee.addEndpoint(&m_X);

  sprintf(buffer, "Target %d Y (millimeter)", m_number);
  m_Y.addAnalogInput();
  m_Y.setAnalogInputDescription(buffer);
  m_Y.setAnalogInputMinMax(0, MAX_DISTANCE_IN_MM);
  Zigbee.addEndpoint(&m_Y);
}

void ZigbeeTarget::Init()
{
  m_X.setAnalogInput(10*m_number);
  m_Y.setAnalogInput(20*m_number);
}

ZigbeeWrapper::ZigbeeWrapper()
  : m_occupancy(1)
  , m_target1(1)
  , m_target2(2)
  , m_target3(3)
{
}

void ZigbeeWrapper::Setup()
{  
  m_occupancy.setManufacturerAndModel(ZIGBEE_MANUFACTURER, ZIGBEE_MODEL);
  Zigbee.addEndpoint(&m_occupancy);


  m_target1.Setup();
  m_target2.Setup();
  m_target3.Setup();

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
  Serial.println("Zigbee is connected");
}

void ZigbeeWrapper::Update()
{
}
