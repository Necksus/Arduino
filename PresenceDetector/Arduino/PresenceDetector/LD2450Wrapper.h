#ifndef LD2450_WRAPPER
#define LD2450_WRAPPER

#include <Arduino.h>
#include <LD2450.hpp>   // https://github.com/Fiooodooor/HLK-LD245X/tree/main
#include <HardwareSerial.h>

using namespace esphome::ld245x;

#define RX_PIN 23
#define TX_PIN 24

class LD2450Wrapper
{
  public:
    LD2450Wrapper();
    void Setup();
    bool HasUpdate();
    RadarTarget GetTarget(uint8_t targetId);

  private:
    HardwareSerial m_Serial;
    LD2450 m_ld2450;
};

#endif