#ifndef ZIGBEE_MODE_ED
#error "Zigbee end device mode is not selected in Tools->Zigbee mode"
#endif

#include "ZigbeeWrapper.h"

#define ZIGBEE_MANUFACTURER     "Fox-Nest Inc"
#define ZIGBEE_MODEL            "foxnest.presence-detector"
#define MAX_DISTANCE_IN_MM      10000

#define TARGET_ENDPOINT_NUMBER   3


ZigbeeTarget::ZigbeeTarget(uint8_t targetId)
  : m_isValid(TARGET_ENDPOINT_NUMBER * (targetId-1) + 2)
  , m_x(TARGET_ENDPOINT_NUMBER * (targetId-1) + 3)
  , m_y(TARGET_ENDPOINT_NUMBER * (targetId-1) + 4)
{
  m_targetId = targetId;
}

void ZigbeeTarget::Setup()
{
  char  buffer[120];

  sprintf(buffer, "Target %d is valid", m_targetId);
  m_isValid.addBinaryInput();
  m_isValid.setBinaryInputApplication(BINARY_INPUT_APPLICATION_TYPE_HVAC_OCCUPANCY);
  m_isValid.setBinaryInputDescription(buffer);
  Zigbee.addEndpoint(&m_isValid);

  sprintf(buffer, "Target %d X (millimeter)", m_targetId);
  m_x.addAnalogInput();
  m_x.setAnalogInputDescription(buffer);
  m_x.setAnalogInputMinMax(-MAX_DISTANCE_IN_MM, MAX_DISTANCE_IN_MM);
  Zigbee.addEndpoint(&m_x);

  sprintf(buffer, "Target %d Y (millimeter)", m_targetId);
  m_y.addAnalogInput();
  m_y.setAnalogInputDescription(buffer);
  m_y.setAnalogInputMinMax(0, MAX_DISTANCE_IN_MM);
  Zigbee.addEndpoint(&m_y);
}

void ZigbeeTarget::Update(bool isValid, int16_t x, int16_t y)
{
  m_isValid.setBinaryInput(isValid);
  m_isValid.reportBinaryInput();
  m_x.setAnalogInput(x);
  m_x.reportAnalogInput();
  m_y.setAnalogInput(y);
  m_y.reportAnalogInput();
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

void ZigbeeWrapper::UpdateTarget(uint8_t targetId, bool isValid, int16_t x, int16_t y)
{
  auto target = GetTarget(targetId);

  if (target != NULL)
  {
    log_d("Target: %d, IsValid: %d, X: %d, Y: %d", targetId, isValid, x, y);
    target->Update(isValid, x, y);
  }
}

void ZigbeeWrapper::UpdateOccupancy(bool occupancy)
{
  m_occupancy.setOccupancy(occupancy);
  m_occupancy.report();
}

ZigbeeTarget* ZigbeeWrapper::GetTarget(uint8_t targetId)
{
  switch(targetId)
  {
    case 0 :
      return &m_target1;
    case 1 :
      return &m_target2;
    case 3 :
      return &m_target3;
    default :
      return NULL;
  }
}
