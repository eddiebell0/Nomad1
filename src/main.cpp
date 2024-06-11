#include <Arduino.h>
#include <FastLED.h> // Library for controlling LED strips
#include <NewPing.h> // Library for ultrasonic sensors

// LED strip configuration
#define NUM_LEDS 121 // Number of LEDs in each strip
#define LED_PIN1 23  // Pin for LED strip 1
#define LED_PIN2 22  // Pin for LED strip 2
#define LED_PIN3 21  // Pin for LED strip 3
#define LED_PIN4 19  // Pin for LED strip 4

CRGB leds1[NUM_LEDS]; // Array to hold LED colors for strip 1
CRGB leds2[NUM_LEDS]; // Array to hold LED colors for strip 2
CRGB leds3[NUM_LEDS]; // Array to hold LED colors for strip 3
CRGB leds4[NUM_LEDS]; // Array to hold LED colors for strip 4

// Ultrasonic sensor configuration
#define MAX_DISTANCE 400 // Maximum distance the sensor can measure (in cm)
#define MIN_DISTANCE 10  // Minimum distance the sensor can measure (in cm)
#define TRIGGER_PIN1 33  // Trigger pin for sensor 1
#define ECHO_PIN1 32     // Echo pin for sensor 1
#define TRIGGER_PIN2 26  // Trigger pin for sensor 2
#define ECHO_PIN2 25     // Echo pin for sensor 2
#define TRIGGER_PIN3 14  // Trigger pin for sensor 3
#define ECHO_PIN3 2      // Echo pin for sensor 3
#define TRIGGER_PIN4 13  // Trigger pin for sensor 4
#define ECHO_PIN4 12     // Echo pin for sensor 4

// Create NewPing objects for each sensor
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

  // Initialize serial communication for debugging (optional)
  Serial.begin(115200);
}

void loop()
{
  // Measure distances using ultrasonic sensors
  unsigned int distance1 = sonar1.ping_cm();
  unsigned int distance2 = sonar2.ping_cm();
  unsigned int distance3 = sonar3.ping_cm();
  unsigned int distance4 = sonar4.ping_cm();

  // Update LED strips based on distances
  updateLEDs(distance1, leds1);
  updateLEDs(distance2, leds2);
  updateLEDs(distance3, leds3);
  updateLEDs(distance4, leds4);

  // Show the LED patterns
  FastLED.show();

  // Small delay to avoid rapid triggering and update LED patterns smoothly
  delay(100);
}

// Function to update LED colors based on distance
// Function to update LED colors based on distance
void updateLEDs(unsigned int distance, CRGB *leds)
{
  // Turn off all LEDs if distance is greater than 100cm or if distance is 0 (no reading)
  if (distance > 100 || distance == 0)
  {
    fill_solid(leds, NUM_LEDS, CRGB::Black); // Turn off all LEDs
  }
  else
  {
    // Calculate the starting LED position for the group of 10 LEDs based on distance
    int ledPosition = map(distance, MIN_DISTANCE, 100, NUM_LEDS - 10, 0); // Inverse mapping
    fill_solid(leds, NUM_LEDS, CRGB::Black);                              // Turn off all LEDs

    // Set colors based on LED position
    for (int i = 0; i < 10; i++)
    {
      if (ledPosition + i <= 30)
        leds[ledPosition + i] = CRGB::Red; // Set color of LED to red for positions 0-30
      else if (ledPosition + i <= 60)
        leds[ledPosition + i] = CRGB::Blue; // Set color of LED to blue for positions 31-60
      else if (ledPosition + i <= 120)
        leds[ledPosition + i] = CRGB::White; // Set color of LED to white for positions 61-120
      else
        leds[ledPosition + i] = CHSV(millis() / 10, 255, 255); // Switching color R, G, B
    }
  }
}
