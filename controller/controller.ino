#include "Adafruit_TCS34725.h"
#include <Adafruit_CircuitPlayground.h>
#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <math.h>

// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true

int servoPin = 9;
Servo servo;
int servoAngle = 0;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
float colors[10][3];
int numColors = 0;

void setup() {
    Serial.begin(115200);
    CircuitPlayground.begin();
    while(!Serial);
    Serial.println("Hello");
    if (tcs.begin()) {
        Serial.println("Found sensor");
    } else {
        Serial.println("No TCS34725 found ... check your connections");
        while (1); // halt!
    }
    servo.attach(servoPin);
    servo.write(0);

    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 3; col++) {
            colors[row][col] = 0;
        }
    }
}


void loop() {
    float red, green, blue;
    tcs.setInterrupt(false);
    delay(60);
    tcs.getRGB(&red, &green, &blue);
    tcs.setInterrupt(true);

    /*Serial.print("R:\t"); Serial.print(int(red)); 
    Serial.print("\tG:\t"); Serial.print(int(green)); 
    Serial.print("\tB:\t"); Serial.print(int(blue));
    Serial.print("\n");*/
    
    for (int row = 0; row < 10; row++) {
        if (fabs(colors[row][0] - red) < 20 && fabs(colors[row][1] - green) < 20 && fabs(colors[row][2] - blue) < 20) {
            servo.write(180);
            delay(10000);
            servo.write(0);
        }
    }

    bool left_first = CircuitPlayground.leftButton();
    bool right_first = CircuitPlayground.rightButton();
    delay(100);
    bool left_second = CircuitPlayground.leftButton();
    bool right_second = CircuitPlayground.rightButton();

    // check if one of buttons was pressed
    if ((left_first && !left_second) || (right_first && !right_second) && numColors < 10) {
        float collarRed, collarGreen, collarBlue;
        // take a color reading of the collar and save the color
        tcs.setInterrupt(false);
        delay(60);
        tcs.getRGB(&collarRed, &collarGreen, &collarBlue);
        tcs.setInterrupt(true);
        colors[numColors][0] = collarRed;
        colors[numColors][1] = collarGreen;
        colors[numColors][2] = collarBlue;
        Serial.println("Added color");

        uint8_t rG = CircuitPlayground.gamma8(collarRed);
        uint8_t gG = CircuitPlayground.gamma8(collarGreen);
        uint8_t bG = CircuitPlayground.gamma8(collarBlue);
        
        
        for (int i = 0; i < 10; ++i) {
          CircuitPlayground.strip.setPixelColor(i, rG, gG, bG);
          String pixel = "Pixel ";
          String setTo = " set to ";
          String comma = ", ";
          String printStr = pixel + i + setTo + rG + comma + gG + comma + bG;
          Serial.println(printStr);
        }
        CircuitPlayground.strip.show();
        numColors++;
        delay(2000);
        CircuitPlayground.clearPixels();
    }
}
