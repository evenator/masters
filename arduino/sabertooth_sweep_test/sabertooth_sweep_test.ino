// Sabertooth motor test script
// Put robot on blocks before running! Will ramp motors up to max speed, then down to negative max speed.
// Ramps motors one at a time, first motor 1, then motor two. Will loop this test forever
// Connect

// Based on Sweep Sample
// Copyright (c) 2012 Dimension Engineering LLC
// See license.txt for license details.

#include <SoftwareSerial.h>
#include <Sabertooth.h>

SoftwareSerial SWSerial(2, 3); // RX on pin 2 (unused), TX on pin 3 (to S1).
Sabertooth ST(130, SWSerial); // Address 130, and use SWSerial as the serial port.

void setup()
{
  SWSerial.begin(9600);
  ST.autobaud();
}

void loop()
{
  int power;
  
  //Ramp motor 1 from 0 to 127 (stop to full forward), waiting 20 ms (1/50th of a second) per value.
  for (power = 0; power <= 127; power ++)
  {
    ST.motor(1, power);
    delay(20);
  }
  
  // Ramp motor 1 down to -127 (full reverse), waiting 20 ms (1/50th of a second) per value
  for (power = 127; power >= -127; power --)
  {
    ST.motor(1, power);
    delay(20);
  }
  
  // Ramp motor 1 down to 0 (stop), waiting 20 ms (1/50th of a second) per value
  for (power = -127; power <= 0; power++)
  {
    ST.motor(1, power);
    delay(20);
  }
  
  //Ramp motor 2 from 0 to 127 (stop to full forward), waiting 20 ms (1/50th of a second) per value.
  for (power = 0; power <= 127; power ++)
  {
    ST.motor(2, power);
    delay(20);
  }
  
  // Ramp motor 2 down from 127 to -127 (full forward to full reverse), waiting 20 ms (1/50th of a second) per value
  for (power = 127; power >= -127; power --)
  {
    ST.motor(2, power);
    delay(20);
  }
  
  // Ramp motor 2 down to 0 (stop), waiting 20 ms (1/50th of a second) per value
  for (power = -127; power <= 0; power++)
  {
    ST.motor(2, power);
    delay(20);
  }
}

