#include "HX711.h"
#include <ArduinoBLE.h>
#include <Arduino.h>

// Define pins used for loadcell
#define D_OUT  2  // Data pin (DT)
#define CLK 3  // Clock pin (SCK)

//ADC Pins
#define ADC1_PIN A0 
#define ADC2_PIN A1
#define ADC3_PIN A2

//PWM Pins
#define PWM_PIN1 5
#define PWM_PIN2 6

// Conductivity sensor variables
float calculated_conductivity;
float calculated_temp;
float calculated_conductivity_standard;
float conductivity;
float resistance;
float v1, v2;

// Constants for conductivity sensor calculations
float kcell = 1.0;
float Temp0 = 25.0;
float R0 = 100.0; //Needs to be measured
float alpha = 0.00393; //degrees C ^ -1
float pwm_voltage;
float TC = 0.02; //Might need to change

//Load cell variables
long calibration_factor = 0;
float known_weight = 10.0;

//Variables needed for bluetooth
// Define BLE service and characteristic
BLEService myService("12345678-1234-5678-1234-56789abcdef0"); // Custom UUID
BLEStringCharacteristic myCharacteristic("abcdefab-cdef-1234-5678-abcdefabcdef", BLERead | BLENotify, 20); // 20-b

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
float calculate_conductivity(float v1, float v2) {
  float v_diff = abs(v1-v2);

	conductivity = kcell/v_diff; //Outputs in microsemens/cm
	return conductivity;
}

float calculate_temperature(float Vt_out) {
  //PWM Voltage per michael 
  pwm_voltage = 0;

  float v_diff = abs(pwm_voltage-Vt_out);

	resistance = v_diff/(1*10^-6); //Resistance in ohms

  Serial.print("Resistance0 for calibration: ");
  Serial.print(resistance);

	calculated_temp = Temp0 + (((resistance/R0) - 1)/alpha);
	return calculated_temp;
}

float calculate_standard_conductivity(float calculated_conductivity, float calculated_temp) {
  float specific_conductance = calculated_conductivity/(1 + TC * (calculated_temp - 25));
  return specific_conductance;
}

// Function to initialize GPIO and ADC pins for the conductivity sensor
void init_cond_pins() {
  //ADC Pins
  pinMode(ADC1_PIN, INPUT);
  pinMode(ADC2_PIN, INPUT);
  pinMode(ADC3_PIN, INPUT);

  //PWM Pin
  pinMode(PWM_PIN1, OUTPUT);
  pinMode(PWM_PIN2, OUTPUT);
}

void init_BLE() {
  delay(2000);
  Serial.println("Starting BLE!");

  if (!BLE.begin()) {
      Serial.println("Failed to start BLE!");
      while (1);
  }

  BLE.setLocalName("NanoESP32_BLE");  // Device name
  BLE.setAdvertisedService(myService);
  myService.addCharacteristic(myCharacteristic);
  BLE.addService(myService);

  BLE.advertise(); // Start advertising
  Serial.println("BLE device ready & advertising...");
}

void setup() {
  Serial.begin(115200);
  scale.begin(D_OUT, CLK);

  //Load cell setup
  init_load_cell();
  handle_user_input_for_calibration();

  //conductivity sensor setup
  init_cond_pins();

  //Initialize BLE
  init_BLE();
}

void transmit_BLE(float data) {
  String strData = String(data, 2);
  myCharacteristic.writeValue(strData);  // Send data back to phone
  Serial.println("Sent: ");
  Serial.println(data);
  delay(1000);
}

void loop() {
  Serial.println("Attemoting to connect BLE!");
  BLEDevice central = BLE.central();
  Serial.print("Connected to: ");
  Serial.println(central.address());

  //Load cell functionality
  float weight = read_weight(); 
  Serial.print("Current weight: ");
  Serial.print(weight);
  Serial.println(" lbs");

  // Initializing PWM
  analogWrite(PWM_PIN1, 127); // 127 = 50% (range is 0-255)
  analogWrite(PWM_PIN2, 127); // 127 = 50% (range is 0-255)

  //Cond. sensor
  v1 = analogRead(ADC1_PIN);
  v2 = analogRead(ADC2_PIN);

  calculated_conductivity = calculate_conductivity(v1, v2); //Potentially look at adding current(a_diff) into this equation

  //Temp sensor
  float Vt_out = analogRead(ADC3_PIN);

  //ASK MSIZZLE ABOUT RESISTORS FOR PWM TO CALC. VOTLAGE OF PWM 
  calculated_temp = calculate_temperature(Vt_out);

  calculated_conductivity_standard = calculate_standard_conductivity(calculated_conductivity, calculated_temp);

  // Print results to Serial Monitor
  Serial.print("Conductivity: ");
  Serial.print(calculated_conductivity);

  Serial.print(" | Temperature: ");
  Serial.println(calculated_temp);


  Serial.print(" | Standard Conductivity: ");
  Serial.println(calculated_conductivity_standard);

  //Data transmission:
  //load cell
  transmit_BLE(weight);

  //Conductivity
  transmit_BLE(calculated_conductivity);
  transmit_BLE(calculated_temp);

  delay(500); 
}