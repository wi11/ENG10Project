#include "Adafruit_TCS34725.h"
#include <Adafruit_CircuitPlayground.h>
#include <Servo.h>
#include <Wire.h>
#include <SPI.h>

// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)
#define redpin 3
#define greenpin 5
#define bluepin 6
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true

// our RGB -> eye-recognized gamma color
byte gammatable[256];

int servoPin = 9;
Servo servo;
int servoAngle = 0;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
float colors[10][3];
int numColors = 0;

void setup() {
    Serial.begin(115200);
    CircuitPlayground.begin();
    if (tcs.begin()) {
        Serial.println("Found sensor");
    } else {
        Serial.println("No TCS34725 found ... check your connections");
        while (1); // halt!
    }
    servo.attach(servoPin);

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

    for (int row = 0; row < 10; row++) {
        if (colors[row][0] == red && colors[row][1] == blue && colors[row][2] == green) {
            servo.write(90);
            delay(60000);
            servo.write(0);
        }
    }

    bool left_first = CircuitPlayground.leftButton();
    bool right_first = CircuitPlayground.rightButton();
    delay(20);
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
        colors[numColors][1] = collarBlue;
        colors[numColors][2] = collarGreen;
        numColors++;
    }
}
