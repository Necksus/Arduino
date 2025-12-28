#ifndef ZIGBEE_WRAPPER
#define ZIGBEE_WRAPPER

#include <Arduino.h>
#include <Zigbee.h>

class ZigbeeTarget
{
  public:
    ZigbeeTarget(uint8_t targetId);
    
    void Setup();
    void Update(bool isValid, int16_t x, int16_t y);

  private:
    uint8_t m_targetId;
    ZigbeeBinary m_isValid;
    ZigbeeAnalog m_x;
    ZigbeeAnalog m_y;    
};


class ZigbeeWrapper
{
  public:
    ZigbeeWrapper();

    void Setup();
    void UpdateTarget(uint8_t targetId, bool isValid, int16_t x, int16_t y);
    void UpdateOccupancy(bool occupancy);

  private:
    ZigbeeOccupancySensor m_occupancy;
    ZigbeeTarget m_target1;
    ZigbeeTarget m_target2;
    ZigbeeTarget m_target3;

    ZigbeeTarget* GetTarget(uint8_t targetId);
};

#endif