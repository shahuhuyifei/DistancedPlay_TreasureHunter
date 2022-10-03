#include "main.h"

// Perform tasks when data received from the other board 
void onRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {

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
  Serial.begin(9600);		// Initialize serial communications with the PC

  // Init ESPNow communication between two boards
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
  WiFi.disconnect();
  ESPNow.init();
  ESPNow.add_peer(playerB_mac);
  ESPNow.reg_recv_cb(onRecv);

	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details

  pixels.begin(); // Initialize neopixel
  pixels.setBrightness(128);
  lightUpLed(black);
  
  initPaj720(); // Initialize Paj7620 registers
}

void loop()
{
  // int hallVal = hallRead();
  // Serial.println(hallVal);
  // if (hallVal > 10 || hallVal < -60) {
  //   piezo.beep(200, 1000);
  //   gestureGame_status = 1;

  //   gestureGame();
  //   piezo.beep(200, 1000);
  // }
  // if (hallVal > 100 || hallVal < 40) {
  //   piezo.beep(200, 1000);
  //   gestureGame();
  //   piezo.beep(200, 1000);
  // }

  
  
  while (treasureCard_uid == NULL)
  {
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
      continue; 
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
    {
      continue;
    }
    // Convert uid to a string 
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      treasureCard_uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      treasureCard_uid.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    treasureCard_uid.toUpperCase();
    treasureCard_uid = treasureCard_uid.substring(1);
    for (int i = 0; i < NUM_CARDS; i++)
    {
    if (treasureCard_uid == playerACards[i])
      {
        outcomingMessage[0] = i + 1; // Plus 1 to ignore the initial 0 value
        delay(100);
        ESPNow.send_message(playerB_mac, outcomingMessage, sizeof(outcomingMessage));
        lightBreath(blue);
      }
    }
  }
  
  // if (cartreasureCard_uidd_uid == playerACards[1])
  // {
  //   // Display blue light and make victory sound
  //   lightUpLed(blue);
  // }
}