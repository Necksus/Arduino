
#include "LD2450Wrapper.h"

#define SERIAL_PORT     1

LD2450Wrapper::LD2450Wrapper()
  : m_Serial(SERIAL_PORT)
{
}

void LD2450Wrapper::Setup()
{
  m_Serial.begin(LD2450_SERIAL_SPEED, SERIAL_8N1, RX_PIN, TX_PIN);
  m_Serial.setTimeout(1000);

  log_d("LD2450, HardwareSerial(1) waiting for sensor data...\n");
  m_ld2450.begin(m_Serial, true);  

  m_ld2450.beginConfigurationSession();
  m_ld2450.setMultiTargetTracking();
  m_ld2450.queryTargetTrackingMode();
  m_ld2450.queryFirmwareVersion();
  m_ld2450.queryMacAddress();
  m_ld2450.queryZoneFilter();
  bool configOk = m_ld2450.endConfigurationSession();
  if (configOk)
  {
    log_d("Sensor name: %s", m_ld2450.getNameString());
    log_d("Zone filter: %s", m_ld2450.getZoneFilter());
    log_d("Firmware value: %s", m_ld2450.getFirmwareString());
    log_d("MacAddress value: %s", m_ld2450.getMacAddressString());
  }
  else
  {
    log_d("Cannot configure LD2450");
  }
}

bool LD2450Wrapper::HasUpdate()
{
  return m_ld2450.update();
}

RadarTarget LD2450Wrapper::GetTarget(uint8_t targetId)
{
  return m_ld2450.getTarget(targetId);
}

/*
void LD2450Wrapper::ProcessLD2450()
{
  if (m_ld2450.update())
  {
    auto target1 = m_ld2450.getTarget(0);
    auto target2 = m_ld2450.getTarget(1);
    auto target3 = m_ld2450.getTarget(2);

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

    
    //Serial.print(m_ld2450.getLastTargetMessage());
    
  }
}
*/