#include <Arduino.h>
#include <FastLED.h> // Library for controlling LED strips
#include <NewPing.h> // Library for ultrasonic sensors

// LED strip configuration
#define NUM_LEDS 121 // Number of LEDs in each strip
#define LED_PIN1 23  // Pin for LED strip 1
#define LED_PIN2 22  // Pin for LED strip 2
#define LED_PIN3 21  // Pin for LED strip 3
#define LED_PIN4 19  // Pin for LED strip 4

CRGB strip1[NUM_LEDS];
CRGB strip2[NUM_LEDS];
CRGB strip3[NUM_LEDS];
CRGB strip4[NUM_LEDS];

// Ultrasonic sensor configuration
#define MAX_DISTANCE 400 // Maximum distance the sensor can measure (in cm)
#define MIN_DISTANCE 10  // Minimum distance the sensor can measure (in cm)
#define TRIGGER_PIN1 33  // Trigger pin for sensor 1
#define ECHO_PIN1 32     // Echo pin for sensor 1
#define TRIGGER_PIN2 26  // Trigger pin for sensor 2
#define ECHO_PIN2 25     // Echo pin for sensor 2
#define TRIGGER_PIN3 14  // Trigger pin for sensor 3
#define ECHO_PIN3 27     // Echo pin for sensor 3
#define TRIGGER_PIN4 13  // Trigger pin for sensor 4
#define ECHO_PIN4 12     // Echo pin for sensor 4

// Create NewPing objects for each sensor
NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE);
NewPing sonar4(TRIGGER_PIN4, ECHO_PIN4, MAX_DISTANCE);

// Function prototype
void updateLEDs(unsigned int distance, CRGB *strip);

void setup()
{
  // Initialize LED strips
  FastLED.addLeds<NEOPIXEL, LED_PIN1>(strip1, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_PIN2>(strip2, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_PIN3>(strip3, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_PIN4>(strip4, NUM_LEDS);

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
  updateLEDs(distance1, strip1);
  updateLEDs(distance2, strip2);
  updateLEDs(distance3, strip3);
  updateLEDs(distance4, strip4);

  // Show the LED patterns
  FastLED.show();

  // Small delay to avoid rapid triggering and update LED patterns smoothly
  delay(100);
}

// Function to blend between two colors
CRGB blendColor(CRGB color1, CRGB color2, float t)
{
  uint8_t r = color1.r + t * (color2.r - color1.r);
  uint8_t g = color1.g + t * (color2.g - color1.g);
  uint8_t b = color1.b + t * (color2.b - color1.b);
  return CRGB(r, g, b);
}

// Function to update LED colors based on distance
void updateLEDs(unsigned int distance, CRGB *strip)
{
  // Turn off all LEDs if distance is greater than 100cm or if distance is 0 (no reading)
  if (distance > 100 || distance == 0)
  {
    fill_solid(strip, NUM_LEDS, CRGB::Black); // Turn off all LEDs
  }
  else
  {
    // Calculate the starting LED position for the group of 10 LEDs based on distance
    int ledPosition = map(distance, MIN_DISTANCE, 100, NUM_LEDS - 10, 0); // Inverse mapping
    fill_solid(strip, NUM_LEDS, CRGB::Black);                             // Turn off all LEDs

    // Set colors based on LED position
    for (int i = 0; i < 10; i++)
    {
      int ledIndex = ledPosition + i;
      if (ledIndex < 60)
      {
        float t = ledIndex / 59.0;                            // Scale t to range from 0 to 1 over the first 60 LEDs
        CRGB color = blendColor(CRGB::Blue, CRGB::Orange, t); // Blend from blue to orange
        strip[ledIndex] = color;
      }
      else if (ledIndex <= 120)
      {
        strip[ledIndex] = CRGB::White; // Set color of LED to white for positions 61-120
      }
      else
      {
        strip[ledIndex] = CRGB::Black; // Default color (black) for any other position
      }
    }
  }
}
