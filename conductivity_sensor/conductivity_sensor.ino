//GPIO pins
#define INPUT1_PIN 2
#define OUTPUT1_PIN 4

//ADC Pins
#define ADC1_PIN A0 
#define ADC2_PIN A1
#define ADC3_PIN A2

//PWM Pins
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

//Conductivity sensor helper functions
float calculate_conductivity(float voltage) {
	conductivity = kcell/voltage;
	return conductivity;
}

float calculate_temperature(float voltage, float current) {
	resistance = voltage/current;
	calculated_temperature = Temp0 + ((resistance - 1)/alpha);
	return calculated_temperature;
}

void setup() {
  Serial.begin(115200);

  //GPIO Inputs
  pinMode(INPUT1_PIN, INPUT);
  pinMode(INPUT2_PIN, INPUT);

  //GPIO Outputs
  pinMode(OUTPUT1_PIN, OUTPUT);

  //ADC Pins
  pinMode(ADC1_PIN, INPUT);
  pinMode(ADC2_PIN, INPUT);
  pinMode(ADC3_PIN, INPUT);

  //PWM Pin
  pinMode(PWM_PIN, OUTPUT);
}

void loop() {
   // Reading GPIO inputs
    int input1 = digitalRead(INPUT1_PIN);
    int input2 = digitalRead(INPUT2_PIN);

    // Read ADC input values
    voltage = analogRead(ADC1_PIN);
    current = analogRead(ADC2_PIN);
    temperature = analogRead(ADC3_PIN);

    // Perform calculations
    calculated_conductivity = calculate_conductivity(voltage);
    calculated_temp = calculate_temperature(voltage, current);

    // Print results to Serial Monitor
    Serial.print("Conductivity: ");
    Serial.print(calculated_conductivity);
    
    Serial.print(" | Temperature: ");
    Serial.println(calculated_temp);

    // Set GPIO Output based on Input 1
    digitalWrite(OUTPUT1_PIN, input1);

    // Generate PWM Signal (50% duty cycle)
    analogWrite(PWM_PIN, 127); // 127 = 50% (range is 0-255)

    delay(500);
}
