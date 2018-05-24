#include <Arduino.h>

#include <ArduinoJson.h>
#include <component_base.h>
#include <component_keyboard_matrix.h>

ComponentBase *keyboardMatrix;

int ledPin = LED_BUILTIN;

StaticJsonBuffer<1200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
//JsonObject& keyboardMatrixJson = root.createObjet()
long count = 0;

void setup() {

    Serial.begin(9600);

    keyboardMatrix = new ComponentKeyboardMatrix(3, 5, 10);


}

void loop() {

    keyboardMatrix->step();
    int kMHasStateChange = keyboardMatrix->getStateChange(jo);
    if(kMHasStateChange) {
        // TODO: Publish the state change over the serial connection
    }
}
