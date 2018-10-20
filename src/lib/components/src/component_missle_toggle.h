#ifndef INCLUDED_COMPONENT_MISSILE_TOGGLE_H
#define INCLUDED_COMPONENT_MISSILE_TOGGLE_H

#include <component_base.h>

#include <Arduino.h>

#include <ArduinoJson.h>

#include <Button.h>

class ComponentMissileToggle : public ComponentBase
{
public:

    enum {
        NUM_TOGGLES = 3,
        NUM_LEDS_PER_TOGGLE = 3
    };

    /*
    * Controller for the missile toggle system.  Assumes three LEDs per toggle
    *
    * @param matrixSize - The size of one side of the (square) matrix
    */
    ComponentMissileToggle( const int signalTapPins[NUM_TOGGLES],
                            const int ledOutputPins[NUM_TOGGLES][NUM_LEDS_PER_TOGGLE],
                            const int lightDelayPerToggle[NUM_TOGGLES]
                        );

    virtual ~ComponentMissileToggle();

    virtual int getStateChange(JsonObject &jsonState);

    virtual void getCurrentState(JsonObject &jsonState) {};

    virtual void step();


private:

    int m_signalTapPins[NUM_TOGGLES];
    int m_led_pins[NUM_TOGGLES][NUM_LEDS_PER_TOGGLE];
    int m_lightDelaysPerToggle[NUM_TOGGLES];

    Button **m_switchList;

    bool m_hasStateChange;

};


#endif
