#include <component_missle_toggle.h>


ComponentMissileToggle::ComponentMissileToggle(
    const int signalTapPins[NUM_TOGGLES],
    const int ledOutputPins[NUM_TOGGLES][NUM_LEDS_PER_TOGGLE],
    const int lightDelayPerToggle[NUM_TOGGLES]) : m_hasStateChange(false)

{
    // Allocate space for switch array
    m_switchList = new[] (Button*)[NUM_TOGGLES];


    //  Init the pins
    for( int ii=0; ii<NUM_TOGGLES; ++ii ) {
        m_signalTapPins[ii] = signalTapPins[ii];
        m_lightDelaysPerToggle[ii] = lightDelayPerToggle[ii];

        //  Not sure of the interface here
        m_switchList[ii] = new Button(m_signalTapPins[ii], INPUT_PULLUP);


        for(int jj=0; jj<NUM_LEDS_PER_TOGGLE; ++jj ) {
            m_led_pins[ii][jj] = ledOutputPins[ii][jj];
        }
    }
}

ComponentMissileToggle::ComponentMissileToggle()
{
    for( int ii=0; ii<NUM_TOGGLES; ++ii) {
        delete m_switchList[ii];
    }
}
