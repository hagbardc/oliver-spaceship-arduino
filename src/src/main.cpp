#include <Arduino.h>

#include <component_button_pad.h>
#include <component_base.h>
#include <ArduinoJson.h>

ComponentBase *buttonPad;
unsigned long count = millis()+1000;


StaticJsonBuffer<1200> jsonBuffer;
JsonObject& jo = jsonBuffer.createObject();


void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);

    Serial.print("Starting Setup...");

    uint8_t colorPins[4][3]  = {{22,24,26}, {30,31,32},{33,34,35},{36,37,38}};
    buttonPad = new ComponentButtonPad(50, 46, 42, colorPins);
    Serial.println("Ending Setup...");
}

void loop() {
    if(millis() >= count) {
        //Serial.println("Calling step");
        count = millis()+1000;
    }

    buttonPad->step(jo);
}
