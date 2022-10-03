#include "main.h"

//  Perform tasks when data received from another board
void onRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{

  if (data[0] != 0 && treasure_card == 0)
  {
    treasure_card = data[0];
    Serial.println(treasure_card);
  }

  if (data[1] == 1)
  {
    otherPlayer_Status = 1;
    Serial.println(otherPlayer_Status);
  }
}

// Ligit up the led strip based on the input color
void lightUpLed(uint32_t color)
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, color);
    pixels.show();
  }
}

void lightBreath(uint32_t color)
{
  for (int i = 0; i <= 128; i++)
  {
    pixels.setBrightness(i);
    lightUpLed(color);
  }
  for (int i = 128; i >= 0; i--)
  {
    pixels.setBrightness(i);
    lightUpLed(color);
  }
  lightUpLed(black);
  pixels.setBrightness(128);
}

// Generates an array of NUM_GESTURES gestures for the gesture game
// 0 is up, 1 is down, 2 is left, 3 is right
int *generateGestures()
{
  static int gesturesArray[NUM_GESTURES];
  for (int i = 0; i < NUM_GESTURES; i++)
  {
    gesturesArray[i] = random(4);
  }
  return gesturesArray;
}

// The gesture game function
void gestureGame()
{
  int *gesturesArray = generateGestures();
  unsigned long startMillis = millis();
  // Loop through all 10 gestures, matching the right gesture
  for (int i = 0; i < NUM_GESTURES; i++)
  {
    bool isNotMatching = true;
    switch (gesturesArray[i])
    {
    case 0: // up
      lightUpLed(red);
      while (isNotMatching)
      {
        isNotMatching = readGesture(GES_UP_FLAG);
      }
      lightUpLed(black);
      piezo.beep(200, 300);
      Serial.println("Match to up!");
      break;
    case 1: // down
      lightUpLed(blue);
      while (isNotMatching)
      {
        isNotMatching = readGesture(GES_DOWN_FLAG);
      }
      lightUpLed(black);
      piezo.beep(200, 300);
      Serial.println("Match to down!");
      break;
    case 2: // left
      lightUpLed(yellow);
      while (isNotMatching)
      {
        isNotMatching = readGesture(GES_LEFT_FLAG);
      }
      lightUpLed(black);
      piezo.beep(200, 300);
      Serial.println("Match to left!");
      break;
    case 3: // right
      lightUpLed(green);
      while (isNotMatching)
      {
        isNotMatching = readGesture(GES_RIGHT_FLAG);
      }
      lightUpLed(black);
      piezo.beep(200, 300);
      Serial.println("Match to right!");
      break;
    }
  }
  lightUpLed(black);
  unsigned long finishMillis = millis();
  gestureGameTime = finishMillis - startMillis;
  Serial.println(gestureGameTime);
}

void setup()
{
  Serial.begin(9600); // Initialize serial communications with the PC

  // Init ESPNow communication between two boards
  WiFi.mode(WIFI_MODE_STA);
  WiFi.disconnect();
  ESPNow.init();
  ESPNow.add_peer(playerA_mac);
  ESPNow.reg_recv_cb(onRecv);

  SPI.begin();                       // Init SPI bus
  mfrc522.PCD_Init();                // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details

  pixels.begin(); // Initialize neopixel
  pixels.setBrightness(128);
  lightUpLed(black);

  initPaj720(); // Initialize Paj7620 registers
}

void loop()
{
  // Loop 3 rounds of the gesture game and guessing
  for (int i = 0; i < ROUNDS; i++)
  {

    while (true)
    {
      // Notify the other player that this player is ready when a magnet touch the hall sensor
      int hallVal = hallRead();
      // Serial.println(hallVal);
      if (hallVal > 50 || hallVal < -60)
      {
        delay(100);
        outcomingMessage[1] = 1;
        ESPNow.send_message(playerA_mac, outcomingMessage, sizeof(outcomingMessage));
        lightBreath(purple);
        break;
      }
    }
    while (true)
    {
      // If the other player is ready, start the gesture game
      if (otherPlayer_Status == 1)
      {
        gestureGame();
        piezo.beep(200, 1000);
        otherPlayer_Status = 0;
        outcomingMessage[1] = 0;
        break;
      }
    }
  }
}