#include "main.h"

void setup()
{
  Serial.begin(9600);

  Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  // Initialize Servos
	gearServo.attach(GEARSERVO_PIN);

  // Setting the outputs of the color sensor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // initialize LED strip WS2812B
  FastLED.addLeds<WS2812B, LED_PIN>(leds, NUM_LEDS);

  // initialize Paj7620 registers
  initPaj720();
}

// Ligits up the led strip based on the input color
void lightUpLed(int *colorArray)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    // Input order is Green, Red, Blue
    leds[i] = CRGB(colorArray[1], colorArray[0], colorArray[2]);
    FastLED.show();
  }
  FastLED.clear();
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
      lightUpLed(colorRed);
      while (isNotMatching)
      {
        isNotMatching = readGesture(GES_UP_FLAG);
      }
      piezo.beep(200, 300);
      Serial.println("Match to up!");
      break;
    case 1: // down
      lightUpLed(colorBlue);
      while (isNotMatching)
      {
        isNotMatching = readGesture(GES_DOWN_FLAG);
      }
      piezo.beep(200, 300);
      Serial.println("Match to down!");
      break;
    case 2: // left
      lightUpLed(colorYellow);
      while (isNotMatching)
      {
        isNotMatching = readGesture(GES_LEFT_FLAG);
      }
      piezo.beep(200, 300);
      Serial.println("Match to left!");
      break;
    case 3: // right
      lightUpLed(colorGreen);
      while (isNotMatching)
      {
        isNotMatching = readGesture(GES_RIGHT_FLAG);
      }
      piezo.beep(200, 300);
      Serial.println("Match to right!");
      break;
    }
  }
  unsigned long finishMillis = millis();
  gestureGameTime = finishMillis - startMillis;
  Serial.println(gestureGameTime);
}

void loop()
{
  // // Rotate to right is 0
  // gearServo.write(0);
  // delay(180);
  // // Stop is 90
  // gearServo.write(90);
  // delay(2000);
  // gearServo.write(0);
  // delay(360);
  // // Stop is 90
  // gearServo.write(90);
  // delay(2000);

  // gestureGame();
  // piezo.beep(200, 1000);
  // delay(1000);

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
  // // Look for new cards
	// if ( ! mfrc522.PICC_IsNewCardPresent()) {
	// 	return;
	// }

	// // Select one of the cards
	// if ( ! mfrc522.PICC_ReadCardSerial()) {
	// 	return;
	// }

  // // Convert uid to a string
  // String card_uid = "";
  // for (byte i = 0; i < mfrc522.uid.size; i++)
  // {
  //   card_uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
  //   card_uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  // }
  // card_uid.toUpperCase();
  // Serial.println(card_uid.substring(1));
}