#include <ArduinoBLE.h>

// Define BLE service and characteristic
BLEService myService("12345678-1234-5678-1234-56789abcdef0"); // Custom UUID
BLEStringCharacteristic myCharacteristic("abcdefab-cdef-1234-5678-abcdefabcdef",
                                         BLERead | BLENotify, 20); // 20-byte max

void setup() {
    Serial.begin(115200);
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

void loop() {
    Serial.println("Attemoting to connect BLE!");
    BLEDevice central = BLE.central();  // Wait for a BLE connection

    if (central) {
        Serial.print("Connected to: ");
        Serial.println(central.address());

        int count = 0;
        while (central.connected()) {
            if (myCharacteristic.written()) {  // Check if data is received
                String receivedData = myCharacteristic.value();  // Read the value
                Serial.print("Value received: ");
                Serial.println(receivedData);
            }

            String funData = "I love Wsully and MSizzle!";
            myCharacteristic.writeValue(funData);  // Send data back to phone
            Serial.println("Sent: " + funData);
            delay(1000);
        }

        Serial.println("Disconnected...");
    }
}