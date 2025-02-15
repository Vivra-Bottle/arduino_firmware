//ADC Pins
#define ADC1_PIN A0 
#define ADC2_PIN A1
#define ADC3_PIN A2
// #define ADC4_PIN A3

//PWM Pins
#define PWM_PIN1 5
#define PWM_PIN2 6

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

void setup() {
  Serial.begin(115200);

  //ADC Pins
  pinMode(ADC1_PIN, INPUT);
  pinMode(ADC2_PIN, INPUT);
  pinMode(ADC3_PIN, INPUT);

  //PWM Pin
  pinMode(PWM_PIN1, OUTPUT);
  pinMode(PWM_PIN2, OUTPUT);
}

void loop() {
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

  delay(500);
}
