#include <SPI.h>
#include <LoRa.h>
#include "MPU6050.h"

MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
unsigned char Tx_command = 0, Speed_index = 0;
#define LORA_CS   10  // NSS
#define LORA_RST  9   // Reset
#define LORA_IRQ  2   // DIO0

void setup() {
    Serial.begin(9600);
    
    // Initialize LoRa module
    LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
    if (!LoRa.begin(433E6)) {  // Set frequency to 433MHz
        Serial.println("LoRa Initialization Failed!");
        while (1);
    }
    Serial.println("LoRa Initialized Successfully!");

    // Initialize MPU6050
    accelgyro.initialize();
}

void loop() {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

   

    if (ay <= -4000) {
        Tx_command = 1;  // Forward
        Speed_index = constrain((-ay - 4000) / 2000 + 1, 1, 5);
    } 
    else if (ay >= 4000) {
        Tx_command = 2;  // Backward
        Speed_index = constrain((ay - 4000) / 2000 + 1, 1, 5);
    } 
    else if (ax <= -4000) {
        Tx_command = 4;  // Right
        Speed_index = constrain((-ax - 4000) / 2000 + 1, 1, 5);
    } 
    else if (ax >= 4000) {
        Tx_command = 3;  // Left
        Speed_index = constrain((ax - 4000) / 2000 + 1, 1, 5);
    } 
    else {
        Tx_command = 0;  // Stop
        Speed_index = 0;
    }

    Serial.print("Sending Command: ");
    Serial.print(Tx_command);
    Serial.print(" | Speed: ");
    Serial.println(Speed_index);

    // Send data using LoRa
    LoRa.beginPacket();
    LoRa.write(Tx_command);
    LoRa.write(Speed_index);
    LoRa.endPacket();

    delay(10);
}
