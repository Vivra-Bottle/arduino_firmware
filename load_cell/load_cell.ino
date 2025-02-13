#include "HX711.h"

// Define pins used for loadcell
#define D_OUT  2  // Data pin (DT)
#define CLK 3  // Clock pin (SCK)

long calibration_factor = 0;
float known_weight = 10.0;

HX711 scale;

void find_calibration_factor() {
  scale.set_scale();
  scale.tare();
  delay(5000);

  long raw_zero = scale.get_units(10);
  Serial.print("Raw zero reading: ");
  Serial.println(raw_zero);
  delay(5000);

  Serial.println("Measuring raw value with the known weight...");
  delay(5000);
  long raw_with_weight = scale.get_units(10);
  Serial.print("Raw reading with weight: ");
  Serial.println(raw_with_weight);

  long change_in_raw_value = raw_with_weight - raw_zero;
  calibration_factor = known_weight / change_in_raw_value;
  Serial.print("Calibration factor: ");
  Serial.println(calibration_factor);
  
  calibration_factor = calibration_factor;
  scale.set_scale(calibration_factor);
}

void setup() {
  Serial.begin(9600);
  scale.begin(D_OUT, CLK);

  Serial.println("Do you need calibration? (y/n)");
  while (Serial.available() == 0) {}

  char user_input = Serial.read();
  user_input = toLowerCase(user_input);

  if (user_input == 'y') {  
    Serial.println("Starting calibration...");
    find_calibration_factor();

    Serial.println("Calibration complete.");
    Serial.print("Calibration factor: ");
    Serial.println(calibration_factor);
  }

}

void loop() {
  float weight = scale.get_units(10); 
  
  Serial.print("Current weight: ");
  Serial.print(weight);
  Serial.println(" lbs");
  
  delay(1000); 
}