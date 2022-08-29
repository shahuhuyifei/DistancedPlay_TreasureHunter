#include "main.h"

void setup()
{
  // // Setting the outputs
  // pinMode(S0, OUTPUT);
  // pinMode(S1, OUTPUT);
  // pinMode(S2, OUTPUT);
  // pinMode(S3, OUTPUT);

  // // Setting the sensorOut as an input
  // pinMode(sensorOut, INPUT);

  // // Setting frequency scaling to 20%
  // digitalWrite(S0, HIGH);
  // digitalWrite(S1, LOW);

  // initialize LED
  FastLED.addLeds<WS2812B, LED_PIN>(leds, NUM_LEDS);

  uint8_t error = 0;

  Serial.begin(9600);

  error = paj7620Init(); // initialize Paj7620 registers
  if (error)
  {
    Serial.print("INIT ERROR,CODE:");
    Serial.println(error);
  }
  else
  {
    Serial.println("INIT OK");
  }
  Serial.println("Please input your gestures:\n");
}

void loop()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    // CRGB ( 0, 0, 255)
    leds[i] = CRGB::Black;
    FastLED.show();
  }

  uint8_t data = 0, data1 = 0, error;

  error = paj7620ReadReg(0x43, 1, &data); // Read Bank_0_Reg_0x43/0x44 for gesture result.
  if (!error)
  {
    switch (data) // When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
    {
    case GES_RIGHT_FLAG:
      paj7620ReadReg(0x43, 1, &data);
      Serial.println("Right");
      break;
    case GES_LEFT_FLAG:
      paj7620ReadReg(0x43, 1, &data);
      Serial.println("Left");
      break;
    case GES_UP_FLAG:
      paj7620ReadReg(0x43, 1, &data);
      Serial.println("Up");
      break;
    case GES_DOWN_FLAG:
      paj7620ReadReg(0x43, 1, &data);
      Serial.println("Down");
      break;
    default:
      paj7620ReadReg(0x44, 1, &data1);
      if (data1 == GES_WAVE_FLAG)
      {
        Serial.println("wave");
      }
      break;
    }
  }
  delay(100);

  // int freq;
  // for (freq = 50; freq <= 6000; freq = freq + 50)
  // {
  //   piezo.beep(freq, 200);
  //   delay(50);
  // }

  // // Setting RED (R) filtered photodiodes to be read
  // digitalWrite(S2, LOW);
  // digitalWrite(S3, LOW);

  // // Reading the output frequency
  // redFrequency = pulseIn(sensorOut, LOW);

  // // Printing the RED (R) value
  // Serial.print("R = ");
  // Serial.print(redFrequency);
  // delay(100);

  // // Setting GREEN (G) filtered photodiodes to be read
  // digitalWrite(S2, HIGH);
  // digitalWrite(S3, HIGH);

  // // Reading the output frequency
  // greenFrequency = pulseIn(sensorOut, LOW);

  // // Printing the GREEN (G) value
  // Serial.print(" G = ");
  // Serial.print(greenFrequency);
  // delay(100);

  // // Setting BLUE (B) filtered photodiodes to be read
  // digitalWrite(S2, LOW);
  // digitalWrite(S3, HIGH);

  // // Reading the output frequency
  // blueFrequency = pulseIn(sensorOut, LOW);

  // // Printing the BLUE (B) value
  // Serial.print(" B = ");
  // Serial.println(blueFrequency);
}

void lightUpLed()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    // CRGB ( 0, 0, 255)
    leds[i] = CRGB::Black;
    FastLED.show();
  }
}