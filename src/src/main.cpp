#include <Arduino.h>

#include <component_sample.h>
#include <ArduinoJson.h>

ComponentSample cs;
int ledPin = LED_BUILTIN;

StaticJsonBuffer<1200> jsonBuffer;
JsonObject& jo = jsonBuffer.createObject();
long count = 0;

void setup() {

    Serial.begin(9600);

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.print("Value is");


}

void loop() {
    if(count == 0) {
        Serial.println("Writing high");
        digitalWrite(ledPin, HIGH);
    }

    if( count % 10000 == 0) {
        Serial.println(count);
    }
    ++count;

    if(count > 50000) {
        count = -50000;
        Serial.println("Writing low");
        digitalWrite(ledPin, LOW);
    }
}
