#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include "secret.h"
#include <ezBuzzer.h>

const int ALARM_PIN = 4;
const int BUZZER_PIN = 5;
const int DELAY_BETWEEN_CARDS = 2000;

//#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(...)   Serial.print (__VA_ARGS__)
  #define DEBUG_PRINTLN(...) Serial.println (__VA_ARGS__)
#else
  #define DEBUG_PRINT(...)
  #define DEBUG_PRINTLN(...) 
#endif

int validTagDetectedNotes[] =
{
    NOTE_F5, NOTE_B5, 0, 0
};
int validTagDetectedDurations[] = 
{
    8, 12, 2, 8
};

int invalidTagDetectedNotes[] =
{
    NOTE_F5, 0
};
int invalidTagDetectedDurations[] = 
{
    1, 2
};


int delayNotificationNotes[] =
{
    NOTE_C6, 0
};
int delayNotificationDurations[] = 
{
    14, 2
};


// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

long timeLastCardRead = 0;
bool readerDisabled = false;
int irqCurr;
int irqPrev;

// This example uses the IRQ line, which is available when in I2C mode.
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
ezBuzzer buzzer(BUZZER_PIN);

void setup(void)
{
  pinMode(ALARM_PIN, INPUT);

#ifdef DEBUG
  Serial.begin(115200);
#endif
  Serial1.begin(115200);

  while (!Serial) delay(10); // for Leonardo/Micro/Zero

  DEBUG_PRINTLN("RFID tag reader is starting...");
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) 
  {
    DEBUG_PRINT("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  DEBUG_PRINT("Found chip PN5"); DEBUG_PRINTLN((versiondata>>24) & 0xFF, HEX);
  DEBUG_PRINT("Firmware ver. "); DEBUG_PRINT((versiondata>>16) & 0xFF, DEC); DEBUG_PRINT('.'); DEBUG_PRINTLN((versiondata>>8) & 0xFF, DEC);

  delay(2000);
  startListeningToNFC();
}

void loop(void)
{
  buzzer.loop();

  // Play delay notification in loop
  if (digitalRead(ALARM_PIN) != 0)
  {
    if (buzzer.getState() == BUZZER_IDLE)
      buzzer.playMelody(delayNotificationNotes, delayNotificationDurations, sizeof(delayNotificationNotes) / sizeof(int));
  }

  if (readerDisabled)
  {
    if (millis() - timeLastCardRead > DELAY_BETWEEN_CARDS) 
    {
      readerDisabled = false;
      startListeningToNFC();
    }
  } 
  else 
  {
    irqCurr = digitalRead(PN532_IRQ);

    // When the IRQ is pulled low - the reader has got something for us.
    if (irqCurr == LOW && irqPrev == HIGH)
    {
       handleCardDetected();
    }

    irqPrev = irqCurr;
  }
}

void startListeningToNFC() 
{
  // Reset our IRQ indicators
  irqPrev = irqCurr = HIGH;

  if (nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)) 
  {
    handleCardDetected();
  }
}

void handleCardDetected()
{
    uint8_t success = false;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // read the NFC tag's info
    success = nfc.readDetectedPassiveTargetID(uid, &uidLength);

    if (success)
    {
      DEBUG_PRINT("Found tag with Id: ");
      nfc.PrintHex(uid, uidLength);

      if (uidLength == 4)
      {
        // We probably have a Mifare Classic card ...
        uint32_t cardid = uid[0];
        cardid <<= 8;
        cardid |= uid[1];
        cardid <<= 8;
        cardid |= uid[2];
        cardid <<= 8;
        cardid |= uid[3];

        // Read block 4 with secret authentication key
        success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
        if (success)
        {
          uint8_t data[16];
          char str[4];

          // Try to read the contents of block 4
          success = nfc.mifareclassic_ReadDataBlock(4, data);
          if (success)
          {
            Serial1.print("{\"PN532\":{\"UID\":\"");
            for(int i=0; i<16; i++)
            {
              sprintf(str,"%x",data[i]);
              Serial1.print(str);
            }
            Serial1.print("\", \"DATA\":\"\"}}\n");

            buzzer.playMelody(validTagDetectedNotes, validTagDetectedDurations, sizeof(validTagDetectedNotes) / sizeof(int));
          }
          else
          {
            DEBUG_PRINTLN("Ooops ... unable to read the requested block.  Try another key?");
            buzzer.playMelody(invalidTagDetectedNotes, invalidTagDetectedDurations, sizeof(invalidTagDetectedNotes) / sizeof(int));
          }
        }
        else
        {
          DEBUG_PRINTLN("Ooops ... authentication failed: Try another key?");
          buzzer.playMelody(invalidTagDetectedNotes, invalidTagDetectedDurations, sizeof(invalidTagDetectedNotes) / sizeof(int));
        }
      }

      timeLastCardRead = millis();
    }

    // The reader will be enabled again after DELAY_BETWEEN_CARDS ms will pass.
    readerDisabled = true;
}
