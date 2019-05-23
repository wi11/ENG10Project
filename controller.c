#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>
#include <tuple>
#include <vector>

void setup() {
    Serial.beign(115200);
    CircuitPlayground.begin();
}

std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> colors;

void loop() {
    uint8_t red, green, blue;
    CircuitPlayground.senseColor(red, green, blue);

    for (auto color : colors) {
        if (std::make_tuple(red, green, blue) == color) {
            // match found
            // TODO: add code to open the lid.
        }
    }

    bool left_first = CircuitPlayground.leftButton();
    bool right_first = CircuitPlayground.rightButton();
    delay(20);
    bool left_second = CircuitPlayground.leftButton();
    bool right_second = CircuitPlayground.rightButton();

    // check if one of buttons was pressed
    if ((left_first && !left_second) || (right_first && !right_second)) {
        CircuitPlayground.clearPixels();

        // take a color reading of the collar and save the color
        CircuitPlayground.senseColor(red, green, blue);
        colors.push_back(std::make_tuple(red, gree, blue));
    }
}
