#include "gesture_functions.h"

// Initialize Paj7620 registers
void initPaj720()
{
  uint8_t gestureError = 0;

  gestureError = paj7620Init();
  if (gestureError)
  {
    Serial.print("INIT gestureError,CODE:");
    Serial.println(gestureError);
  }
  else
  {
    Serial.println("INIT GESTURE OK");
  }
}

// Reads one gesture input
bool readGesture(uint8_t GES)
{
  uint8_t gestureData = 0, gestureError;
  gestureError = paj7620ReadReg(0x43, 1, &gestureData); // Read Bank_0_Reg_0x43/0x44 for gesture result.
  if (!gestureError && gestureData == GES)
  {
    return false;
  }
  return true;
}
