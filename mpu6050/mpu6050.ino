// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"
 
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
const char* ssid     = "Azzam";
const char* password = "azzam53a";
const char* host = "192.168.1.23";
WiFiClient client;   

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
 
int16_t ax, ay, az;
int16_t gx, gy, gz;
 
#define LED_PIN 13
bool blinkState = false;
 
void setup() {
// join I2C bus (I2Cdev library doesn't do this automatically)
Wire.begin();

// initialize serial communication
// (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
// it's really up to you depending on your project)
Serial.begin(9600);

// We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
// initialize device
Serial.println("Initializing I2C devices...");
accelgyro.initialize();
 
// verify connection
Serial.println("Testing device connections...");
Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
 
// configure Arduino LED for
pinMode(LED_PIN, OUTPUT);
}
 
void loop() {

// read raw accel/gyro measurements from device
accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

String val_ax, val_ay, val_az, val_gx, val_gy, val_gz;
val_ax = String(ax); val_ay = String(ay); val_az = String(az);
val_gx = String(gx); val_gy = String(gy); val_gz = String(gz);

// these methods (and a few others) are also available
//accelgyro.getAcceleration(&ax, &ay, &az);
//accelgyro.getRotation(&gx, &gy, &gz);

// display tab-separated accel/gyro x/y/z values
Serial.print("a/g:\t");
Serial.print(val_ax); Serial.print("\t");
Serial.print(val_ay); Serial.print("\t");
Serial.print(val_az); Serial.print("\t");
Serial.print(val_gx); Serial.print("\t");
Serial.print(val_gy); Serial.print("\t");
Serial.println(val_gz);

delay(3000); 

Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

 
    // This will send the request to the server
 client.print(String("GET http://192.168.1.23/iot-s2/mpu6050.php?") + 
                          ("ax=") + ax +
                          ("&ay=") + ay +
                          ("&az=") + az +
                          ("&gx=") + gx +
                          ("&gy=") + gy +
                          ("&gz=") + gz +
                          " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 1000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        
    }

    Serial.println();
    Serial.println("closing connection");

// blink LED to indicate activity
blinkState = !blinkState;
digitalWrite(LED_PIN, blinkState);
}
