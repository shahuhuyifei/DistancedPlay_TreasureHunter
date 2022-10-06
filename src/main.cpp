#include "main.h"

//  Perform tasks when data received from another board
void onRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
  // When the data is the variable outcomingMessage
  if (data_len == 2)
  {
    // Store the treasure card number when it's available
    if (data[0] != 0 && treasure_card == 0)
    {
      treasure_card = data[0];
      Serial.print("Treasure Card: ");
      Serial.println(treasure_card);
    }
    // Store the other player gesture game status when it's available
    if (data[1] == 1)
    {
      otherPlayer_Status = 1;
      Serial.println("Other Player Status: ");
      Serial.println(otherPlayer_Status);
    }
  }
  else
  {
    // Convert the gesture game result array back to an unsigned long
    for (int i = 0; i < data_len; i++)
    {
      otherPlayer_result += data[i] * pow(10, i);
    }
    Serial.print("Other Player result: ");
    Serial.println(otherPlayer_result);
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
unsigned long gestureGame()
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
  return gestureGameTime;
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
      Serial.println(otherPlayer_Status);
      if (otherPlayer_Status == 1)
      {
        gestureGame_result = gestureGame();
        piezo.beep(200, 1000);
        otherPlayer_Status = 0;
        outcomingMessage[1] = 0;
        unsigned long resultForCalculate = gestureGame_result;
        // Convert gesture game result into uint8_t array to send
        int resultLength = int(log10(resultForCalculate) + 1);
        uint8_t gestureResultArray[resultLength];
        // Source of next 7 line: https://forum.arduino.cc/t/long-integer-to-uint8_t/348706/2
        // Modified by Yifei Wang
        for (int i = 0; i < resultLength; i++)
        {
          gestureResultArray[i] = resultForCalculate % 10; // Remainder of division with 10 gets the last digit
          resultForCalculate /= 10;                        // Dividing by ten chops off the last digit
        }
        Serial.println();
        // -------------------------------------------------------------
        ESPNow.send_message(playerA_mac, gestureResultArray, sizeof(gestureResultArray));
        break;
      }
    }
    while (true)
    {
      Serial.println(otherPlayer_result);
      // Wait for the other player's result come through
      if (otherPlayer_result != 0)
      {
        if (otherPlayer_result > gestureGame_result) // Win
        {
          lightBreath(green);
        }
        else if (otherPlayer_result == gestureGame_result) // Even
        {
          lightBreath(yellow);
        }
        else // Lose
        {
          lightBreath(red);
        }
        otherPlayer_result = 0;
        break;
      }
    }
    // Guess where the treasure is by tapping a card
    while (guessedCard_uid == "")
    {
      Serial.println("here!");
      // Look for new cards
      if (!mfrc522.PICC_IsNewCardPresent())
      {
        continue;
      }
      // Select one of the cards
      if (!mfrc522.PICC_ReadCardSerial())
      {
        continue;
      }
      // Convert uid to a string
      for (byte i = 0; i < mfrc522.uid.size; i++)
      {
        guessedCard_uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        guessedCard_uid.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      guessedCard_uid.toUpperCase();
      guessedCard_uid = guessedCard_uid.substring(1);
      // Match the card number
      for (int i = 0; i < NUM_CARDS; i++)
      {
        if (guessedCard_uid == playerBCards[i])
        {
          // Send the guessed card
          outcomingMessage[0] = i + 1; // Plus 1 to ignore the initial 0 value
          delay(100);
          ESPNow.send_message(playerA_mac, outcomingMessage, sizeof(outcomingMessage));
          if (i + 1 == treasure_card)
          {
            lightBreath(green); // Correct - Win
          }
          else
          {
            lightBreath(red); // Incorrect - Lose
          }
        }
      }
    }
    guessedCard_uid = "";
    outcomingMessage[0] = 0;
  }
}