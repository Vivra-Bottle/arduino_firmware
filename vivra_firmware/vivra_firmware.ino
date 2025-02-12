/*
  Author: Mathew Maradin
*/

//GPIO pins
#define INPUT1_PIN 2
#define OUTPUT1_PIN 4

//ADC Pins
#define ADC1_PIN A0 
#define ADC2_PIN A1
#define ADC3_PIN A2

//PWM Pins
#define PWM_PIN 5

// Load cell variables
int tare = 0;
float knownOriginal = 1; // in milli gram
float knownHX711 = 1;
int weight;

// Conductivity sensor variables
uint16_t temperature = 0;
uint16_t voltage = 0;
uint16_t current = 0;
char temperatures[20];
float calculated_conductivity;
float calculated_temp;
float test1, test2;

// HX711 helper functions
// void microDelay(int delay)
// {
//   __HAL_TIM_SET_COUNTER(&htim2, 0);
//   while (__HAL_TIM_GET_COUNTER(&htim2) < delay);
// }

// int getHX711(void)
// {
//   uint32_t data = 0;
//   uint32_t startTime = HAL_GetTick();
//   while(HAL_GPIO_ReadPin(DT_PORT, DT_PIN) == GPIO_PIN_SET)
//   {
//     if(HAL_GetTick() - startTime > 200)
//       return 0;
//   }
//   for(int len=0; len<24 ; len++)
//   {
//     HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_SET);
//     microDelay(1);
//     data = data << 1;
//     HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_RESET);
//     microDelay(1);
//     if(HAL_GPIO_ReadPin(DT_PORT, DT_PIN) == GPIO_PIN_SET)
//       data ++;
//   }
//   data = data ^ 0x800000;
//   HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_SET);
//   microDelay(1);
//   HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_RESET);
//   microDelay(1);
//   return data;
// }

// int weigh()
// {
//   int  total = 0;
//   int  samples = 50;
//   int milligram;
//   float coefficient;
//   for(uint16_t i=0 ; i<samples ; i++)
//   {
//       total += getHX711();
//   }
//   int32_t average = (int32_t)(total / samples);
//   coefficient = knownOriginal / knownHX711;
//   milligram = (int)(average-tare)*coefficient;
//   return milligram;
// }

// //Conductivity sensor helper functions
// float calculate_conductivity(int voltage)
// {
// 	conductivity = kcell/voltage;

// 	return conductivity;
// }

// float calculate_temperature(int voltage, int current)
// {
// 	resistance = voltage/current;
// 	calculated_temperature = Temp0 + ((resistance - 1)/alpha);

// 	return calculated_temperature;
// }

void setup() {
  Serial.begin(115200); //Is this needed

  //GPIO Inputs
  PinMode(INPUT1_PIN, INPUT);
  PinMode(INPUT2_PIN, INPUT);

  //GPIO Outputs
  PinMode(OUTPUT1_PIN, OUTPUT);

  //ADC Pins
  PinMode(ADC1_PIN, INPUT);
  PinMode(ADC2_PIN, INPUT);
  PinMode(ADC3_PIN, INPUT);

  //PWM Pin
  PinMode(PWM_PIN, OUTPUT);
}

void loop() {
    // Reading GPIO inputs
    int PD_SCK = digitalRead(INPUT1_PIN);

    // Read ADC input values
    int V_Diff = analogRead(ADC1_PIN);
    int A_Diff = analogRead(ADC2_PIN);
    int T_Diff = analogRead(ADC3_PIN);

    // Set GPIO Output based on Input 1
    digitalWrite(OUTPUT_PIN1, input1);

    // Generate PWM Signal (50% duty cycle)
    analogWrite(PWM_PIN, 127); // 127 = 50% (range is 0-255)

    //TEST Printing
    // Serial.print("Input1: "); Serial.print(input1);
    // Serial.print(" | Input2: "); Serial.print(input2);
    // Serial.print(" | ADC1: "); Serial.print(adc1_value);
    // Serial.print(" | ADC2: "); Serial.print(adc2_value);
    // Serial.print(" | ADC3: "); Serial.println(adc3_value);

    // delay(500);
}
