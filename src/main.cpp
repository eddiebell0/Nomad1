#include <Arduino.h>
#include <FastLED.h>
#include <NewPing.h>

// LED strip configuration
#define NUM_LEDS 121
#define LED_PIN1 23
#define LED_PIN2 22
#define LED_PIN3 21
#define LED_PIN4 19

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];
CRGB leds4[NUM_LEDS];

// Ultrasonic sensor configuration
#define MAX_DISTANCE 400
#define MIN_DISTANCE 10
#define TRIGGER_PIN1 33
#define ECHO_PIN1 32
#define TRIGGER_PIN2 26
#define ECHO_PIN2 25
#define TRIGGER_PIN3 14
#define ECHO_PIN3 2
#define TRIGGER_PIN4 13
#define ECHO_PIN4 12

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE);
NewPing sonar4(TRIGGER_PIN4, ECHO_PIN4, MAX_DISTANCE);

// Function prototype
void updateLEDs(unsigned int distance, CRGB *leds);

void setup()
{
  // Initialize LED strips
  FastLED.addLeds<NEOPIXEL, LED_PIN1>(leds1, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_PIN2>(leds2, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_PIN3>(leds3, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_PIN4>(leds4, NUM_LEDS);

  // Initialize serial communication
  Serial.begin(115200);
}

void loop()
{
  // Measure distances
  unsigned int distance1 = sonar1.ping_cm();
  unsigned int distance2 = sonar2.ping_cm();
  unsigned int distance3 = sonar3.ping_cm();
  unsigned int distance4 = sonar4.ping_cm();

  // Update LED strips based on distances
  updateLEDs(distance1, leds1);
  updateLEDs(distance2, leds2);
  updateLEDs(distance3, leds3);
  updateLEDs(distance4, leds4);

  // Show the LEDs
  FastLED.show();

  // Small delay to avoid rapid triggering
  delay(100);
}

void updateLEDs(unsigned int distance, CRGB *leds)
{
  // Turn off all LEDs if distance is greater than 100cm
  if (distance > 100 || distance == 0)
  {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  }
  else
  {
    // Calculate the position for the group of 10 LEDs
    int ledPosition = map(distance, MIN_DISTANCE, 100, 0, NUM_LEDS - 10);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    for (int i = 0; i < 10; i++)
    {
      leds[ledPosition + i] = CRGB::White;
    }
  }
}
