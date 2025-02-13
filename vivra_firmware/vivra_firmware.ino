#include "HX711.h"

// Define pins used for loadcell
#define D_OUT  2  // Data pin (DT)
#define CLK 3  // Clock pin (SCK)

// Define GPIO pins
#define INPUT1_PIN 2
#define OUTPUT1_PIN 4

// ADC Pins
#define ADC1_PIN A0 
#define ADC2_PIN A1
#define ADC3_PIN A2

// PWM Pin
#define PWM_PIN 5

// Conductivity sensor variables
float temperature = 0;
float voltage = 0;
float current = 0;
float calculated_conductivity;
float calculated_temp;

// Constants for conductivity and temperature calculation
float kcell = 1.0;
float Temp0 = 25.0;
float alpha = 0.02;

//Load cell variables
long calibration_factor = 0;
float known_weight = 10.0;

HX711 scale;

void init_load_cell() {
  scale.begin(D_OUT, CLK);
  scale.set_scale();
  scale.tare();
  Serial.println("Scale initialized.");
}

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

float read_weight() {
  return scale.get_units(10);
}

void handle_user_input_for_calibration() {
  Serial.println("Do you need calibration? (y/n)");

  while (Serial.available() == 0) {}

  char user_input = Serial.read();
  user_input = toLowerCase(user_input);

  if (user_input == 'y') {
    find_calibration_factor();
  }
}

// Conductivity sensor helper functions
float calculate_conductivity(float voltage) {
  return kcell / voltage;
}

float calculate_temperature(float voltage, float current) {
  float resistance = voltage / current;
  return Temp0 + ((resistance - 1) / alpha);
}

// Function to initialize GPIO and ADC pins for the conductivity sensor
void init_gpio() {
  // GPIO Inputs
  pinMode(INPUT1_PIN, INPUT);
  pinMode(INPUT2_PIN, INPUT);

  // GPIO Outputs
  pinMode(OUTPUT1_PIN, OUTPUT);

  // ADC Pins
  pinMode(ADC1_PIN, INPUT);
  pinMode(ADC2_PIN, INPUT);
  pinMode(ADC3_PIN, INPUT);

  // PWM Pin
  pinMode(PWM_PIN, OUTPUT);
}

void setup() {
  Serial.begin(9600);
  scale.begin(D_OUT, CLK);

  //Load cell setup
  init_load_cell();
  handle_user_input_for_calibration();

  //conductivity sensor setup
  init_gpio();
  }

}

void loop() {
  //Load cell functionality
  float weight = read_weight(); 
  Serial.print("Current weight: ");
  Serial.print(weight);
  Serial.println(" lbs");

  // Read GPIO inputs for conductivity sensor
  int input1 = digitalRead(INPUT1_PIN);
  int input2 = digitalRead(INPUT2_PIN);

  // Read ADC input values for conductivity sensor
  voltage = analogRead(ADC1_PIN);
  current = analogRead(ADC2_PIN);
  temperature = analogRead(ADC3_PIN);

  // Perform conductivity and temperature calculations
  calculated_conductivity = calculate_conductivity(voltage);
  calculated_temp = calculate_temperature(voltage, current);

  // Print conductivity and temperature readings
  Serial.print("Conductivity: ");
  Serial.print(calculated_conductivity);
  Serial.print(" | Temperature: ");
  Serial.println(calculated_temp);

  // Set GPIO Output based on Input 1 (for the conductivity sensor)
  digitalWrite(OUTPUT1_PIN, input1);

  // Generate PWM Signal (50% duty cycle)
  analogWrite(PWM_PIN, 127); // 127 = 50% (range is 0-255)

  delay(500); 
}