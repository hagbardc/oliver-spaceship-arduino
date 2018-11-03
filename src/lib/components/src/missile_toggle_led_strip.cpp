#include <missile_toggle_led_strip.h>

#include <Arduino.h>


unsigned long MissileToggleLedStrip::s_blinkDelay = 125;  // in ms

MissileToggleLedStrip::MissileToggleLedStrip(   int signal_pin, int numLeds, int ledPins[],
                                                unsigned long millisecDelay)
                                                    :   m_numberOfLeds(numLeds),
                                                        m_signalTap(signal_pin),
                                                        m_toggleOnTimestamp(0),
                                                        m_millisecDelay(millisecDelay)
{
    //  Allocate space for the led arrays
    m_pinArray = new int[m_numberOfLeds];
    m_pinArrayState = new int[m_numberOfLeds];

    for(int ii=0; ii<m_numberOfLeds; ++ii) {
        m_pinArray[ii] = ledPins[ii];
        pinMode(m_pinArray[ii], OUTPUT);
    }


    pinMode(m_signalTap, INPUT);

    //  We can't start the system in an activated state
    if( digitalRead(m_signalTap) == HIGH ) {
        m_mode = ToggleMode::INVALID_MODE;
    } else {
        m_mode = ToggleMode::WAITING_MODE;
    }
}

MissileToggleLedStrip::~MissileToggleLedStrip()
{
    delete[] m_pinArray;
    delete[] m_pinArrayState;
}

void MissileToggleLedStrip::init()
{
    for(int ii=0; ii<m_numberOfLeds; ++ii) {
        setPinState(ii, LOW);
    }

    for(int ii=0; ii<m_numberOfLeds; ++ii) {
        setPinState(ii, HIGH);
        delay(s_blinkDelay);
    }

    for(int ii=0; ii<m_numberOfLeds; ++ii) {
        setPinState(ii, LOW);
        delay(s_blinkDelay);
    }

    //  Blink the lights on and off 3 times
    for(int blinkLoop=0; blinkLoop<3; ++blinkLoop) {
        for(int ii=0; ii<m_numberOfLeds; ++ii) {
            setPinState(ii, HIGH);
        }
        delay(s_blinkDelay*2);

        for(int ii=0; ii<m_numberOfLeds; ++ii) {
            setPinState(ii, LOW);
        }
        delay(s_blinkDelay*2);
    }

    //  Calling init while switches are on results in an error mode
    if( digitalRead(m_signalTap) == HIGH ) {
        m_mode = ToggleMode::INVALID_MODE;
    } else {
        m_mode = ToggleMode::WAITING_MODE;
    }
}

void MissileToggleLedStrip::step()
{
    m_toggleState = digitalRead(m_signalTap);

    switch(m_mode)
    {
        case ToggleMode::INVALID_MODE:
        {
            invalid_step();
            break;
        }
        case ToggleMode::ACTIVE_MODE:
        {
            active_mode_step();
            break;
        }
        case ToggleMode::PROCESSING_MODE:
        {
            processing_mode_step();
            break;
        }
        case ToggleMode::RESET_FROM_ERROR_MODE:
        case ToggleMode::WAITING_MODE:
        default:
        {
            waiting_mode_step();
            break;
        }
    }
    return;
}


void MissileToggleLedStrip::invalid_step()
{
    // If the toggle is OFF, turn off the LEDs, reset the system to waiting, and return
    // If this stays here, it means that an external source can't keep this in INVALID mode
    // when the switch is off....

    // If the toggle is ON, continue with a blink
    // We reuse the 'toggleOnTimestamp' here
    if(0 == m_toggleOnTimestamp) {
        m_toggleOnTimestamp = millis();
        return;
    }

    unsigned long volatile currentTime = millis();
    unsigned long volatile timeDelaySinceStateChange = currentTime - m_toggleOnTimestamp;

    // If the time delay has exceeded our delay, switch the state of the LEDS
    // Reset the 'toggleOnTimestamp' to reset delay timer
    if( timeDelaySinceStateChange > s_blinkDelay) {
        int stateToSet = m_pinArrayState[0] ? LOW : HIGH;
        for(int ii=0; ii<m_numberOfLeds; ++ii) {
            setPinState(ii, stateToSet);
        }
        m_toggleOnTimestamp = currentTime;
    }
}

void MissileToggleLedStrip::reset_from_error_mode_step()
{
    return;
}

//  The state wasn't changed externally, and we just need to be silent
void MissileToggleLedStrip::waiting_mode_step()
{
    if(!m_toggleState) {
        if(m_toggleOnTimestamp) {
            resetToWaitingMode();
        }
    }
    else { //  Really we shouldn't get to this code.  So we should just print an error
        Serial.println("Error: component in WAITING_MODE but toggle switch is ON");
    }
    return;
}

// In this case, the LEDs should start turning on.  Once this process is complete,
// this class should move the state to ACTIVE_MODE
void MissileToggleLedStrip::processing_mode_step()
{
    if(0 == m_toggleOnTimestamp) {
        m_toggleOnTimestamp = millis();
        return;
    }

    // Calculate time between now and when the toggle switch was turned on.
    // Turn on LEDs depending on how far from that time we are
    unsigned long volatile currentTime = millis();
    unsigned long volatile timeDelaySinceOn = currentTime - m_toggleOnTimestamp;


    // Time delays are: [0, 2T), [2T, 3T), [3T, 4T), etc
    //  Use this loop to determine which time delay we should use
    for( int ii=0; ii<m_numberOfLeds; ++ii) {

        if( ii==0 &&
            timeDelaySinceOn > m_millisecDelay &&
            timeDelaySinceOn < (2*m_millisecDelay) ) {
                setPinState(ii, 1);
        }

        //  This covers the middle cases
        if( ii != (m_numberOfLeds-1) &&
            timeDelaySinceOn > ((ii+1)*m_millisecDelay) ) {
            setPinState(ii, 1);
        }

        if(ii==(m_numberOfLeds-1)) {
            setPinState(ii, 1);
            m_mode = ToggleMode::ACTIVE_MODE;  // Only move to active when the last LED has turned on
        }
    }
}


void MissileToggleLedStrip::resetToWaitingMode()
{
    for( int ii=0; ii<m_numberOfLeds; ++ii) {
        setPinState(ii, 0);
        m_toggleOnTimestamp = 0;
        m_mode = ToggleMode::WAITING_MODE;
    }
}

// The only thing we'll do in active move is check whether the toggle is still on
void MissileToggleLedStrip::active_mode_step()
{
    if(!m_toggleState) {
        resetToWaitingMode();
    }
    return;
}


void MissileToggleLedStrip::setPinState(int pinIndex, int onoff)
{
    //  In the case of a "TURN ON PIN"
    if(onoff) {
        if(! m_pinArrayState[pinIndex]) {
            digitalWrite(m_pinArray[pinIndex], HIGH);
            m_pinArrayState[pinIndex] = 1;
        }
    }
    else {  // In the case of a "TURN OFF PIN"
        if(m_pinArrayState[pinIndex]) {
            digitalWrite(m_pinArray[pinIndex], LOW);
            m_pinArrayState[pinIndex] = 0;
        }
    }
}

void MissileToggleLedStrip::setMode(MissileToggleLedStrip::ToggleMode newMode)
{
     m_mode = newMode;
}
