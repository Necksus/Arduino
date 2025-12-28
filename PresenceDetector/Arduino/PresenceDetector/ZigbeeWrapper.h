#ifndef ZIGBEE_WRAPPER
#define ZIGBEE_WRAPPER

#include <Arduino.h>
#include <Zigbee.h>

class ZigbeeTarget
{
  public:
    ZigbeeTarget(uint8_t number);
    
    void Setup();
    void Init();

  private:
    uint8_t m_number;
    ZigbeeBinary m_IsValid;
    ZigbeeAnalog m_X;
    ZigbeeAnalog m_Y;
};


class ZigbeeWrapper
{
  public:
    ZigbeeWrapper();

    void Setup();
    void Update();

  private:
    ZigbeeOccupancySensor m_occupancy;
    ZigbeeTarget m_target1;
    ZigbeeTarget m_target2;
    ZigbeeTarget m_target3;
};

#endif