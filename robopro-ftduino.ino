/*
 * ROBO Pro für den ftDuino v1.0
 * Parser für das fish.X1-Protokoll des TX Controllers
 * (TX Firmware 1.30)
 * 
 * Einschränkungen:
 * - Ultraschall-Abstandssensor nicht unterstützt
 * - keine BT-Unterstützung (work in progress)
 * - keine Synchronisation von Motoren
 * - keine Kaskadierung von ftDuinos ("Extensions") möglich
 * 
 * Bibliotheken:
 * - ftduino.h
 * - wire.h
 * 
 * Dirk Fox
 * 
 * - 07.09.2022: publication of v1.1 (I2C support, correction of line sensor values)
 * - 02.09.2022: publication of v1.0
 * - 27.07.2022: Fork of fx1sample 0.3 by ft-ninja
 */

#include <Ftduino.h>
#include <Wire.h> 
#include "fishx1.h"

// enabling bluetooth serial will disable I2c support in this sketch as
// the btserial adapter itself is connected via i2c
#define ENABLE_BTSERIAL

#ifdef ENABLE_BTSERIAL
#include "I2cSerialBt.h"
I2cSerialBt btSerial;
bool use_bt = false;
#endif

void setup()
{
  Serial.begin(TX_BAUD);  // ROBO TX Controller Serial UART (38400,8,n,1)
  ftduino.init();         // initialize ftDuino API

#ifndef ENABLE_BTSERIAL
  Wire.begin();           // initialize I2C driver, join I2C bus as master (controller)
#else
  btSerial.begin(38400);  // the HC-05 needs to be prepared for the bitrate
  Wire.setClock(400000);  // well beyond the spec ...
  btSerial.key(0);
#endif

  // wait a short moment for Serial (USB) to become available. Switch
  // to bluetooth if it doesn't
  uint32_t to = millis();
  while (!Serial && millis()-to < 1000 );
  if(!Serial) use_bt = true;
}

#ifdef ENABLE_BTSERIAL
void fx1_write(unsigned char byte) {
  if(!use_bt)
    Serial.write(byte);
  else
    btSerial.write(byte);
} 
#else
#define write(a)   Serial.write(a)
#endif

void loop()
{
  if (!use_bt && Serial && Serial.available() > 0) // check UART and read one symbol
  {
#ifdef ENABLE_BTSERIAL
    use_bt = false;
#endif
    fx1Parse(Serial.read());  // parse Fish.X1 protocol
  } 

#ifdef ENABLE_BTSERIAL
  unsigned char available = btSerial.available();
  if (available > 0) 
  {
    use_bt = true;
    unsigned char buffer[available];
    
    btSerial.read(available, buffer);

    for(char i=0;i<available;i++)
      fx1Parse(buffer[i]); 
  } 
#endif
}
