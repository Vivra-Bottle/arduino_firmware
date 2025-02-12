#include "HX711.h"

// Define HX711 pins
#define DT_PIN  3  // Data pin (DT)
#define SCK_PIN 2  // Clock pin (SCK)

HX711 scale;

void setup() {
    Serial.begin(9600);  // Start serial communication
    scale.begin(DT_PIN, SCK_PIN);  // Initialize HX711

    Serial.println("Initializing scale...");

    if (!scale.is_ready()) {
        Serial.println("HX711 not found. Check wiring.");
        while (1);  // Stop execution
    }

    Serial.println("Place a known weight on the scale to calibrate.");
    delay(5000);  // Wait 5 seconds for user input
    long zeroOffset = scale.read_average(10);  // Get the zero offset
    Serial.print("Zero offset: ");
    Serial.println(zeroOffset);

    scale.set_scale(2280.f);  // Set scale factor (Adjust based on calibration)
    scale.tare();  // Reset the scale to zero

    Serial.println("Scale ready!");
}

void loop() {
    if (scale.is_ready()) {
        float weight = scale.get_units(10);  // Read weight in grams
        Serial.print("Weight: ");
        Serial.print(weight, 2);
        Serial.println(" g");
    } else {
        Serial.println("HX711 not ready");
    }

    delay(500);  // Wait before next reading
}