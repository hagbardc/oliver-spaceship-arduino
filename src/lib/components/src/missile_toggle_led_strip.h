#ifndef INCLUDED_MISSILE_TOGGLE_LED_STRIP
#define INCLUDED_MISSILE_TOGGLE_LED_STRIP


/*
 * @brief Class which controls state machine for toggle switch and N leds
 *        When switch is on, the three LEDs start to turn on with specified delay
 *
 */
class MissileToggleLedStrip
{
public:

    typedef enum ToggleMode
    {
        RESET_FROM_ERROR_MODE,  // Switch was turned off, but was previously in an error state
        WAITING_MODE,  // Switch is not in ON state, external call has not set to INVALID
        INVALID_MODE,  // External call has set this instance to invalid - needs to be reset externally
        PROCESSING_MODE, // For when switch has been properly turned on, but LEDs are not completely lit
        ACTIVE_MODE    // Switch has been activated properly (ON state)
    } ToggleMode_t;

    MissileToggleLedStrip(  int signal_pin, int numLeds, int ledPins[],
                            unsigned long millisecDelay = 500);

    ~MissileToggleLedStrip();

    //  Visual initialization sequence.
    //  This blocks (uses delays() to control blinking and whatnot)
    void init();


    //  Executes the read process, and keeps track of the time at which the switch
    //  was thrown.  Starts lighting LEDS at timed intervals, until all three are lit
    void step();


    void setMode(MissileToggleLedStrip::ToggleMode newMode);
    MissileToggleLedStrip::ToggleMode getMode() { return m_mode; }

    int getSignalTap() { return m_signalTap; }

    // Turns off the pins, resets the toggle on timestamp, sets m_mode to WAITING_MODE
    void resetToWaitingMode();

private:

    static unsigned long s_blinkDelay; // Length of time for a LED blink

    //  Turns a pin on or off as requested.  We keep track of pin state, so extraneous
    //  reads are not done
    void setPinState(int pinIndex, int onoff);

    //  Progresses the action forward one step, depending on the state
    void invalid_step();
    void waiting_mode_step();
    void reset_from_error_mode_step();
    void active_mode_step();
    void processing_mode_step();


    int m_numberOfLeds;
    int m_signalTap;
    int *m_pinArray;
    int *m_pinArrayState;  //  The current state of the pins


    bool m_toggleState;
    unsigned long m_toggleOnTimestamp;  // time (in ms) toggle was switched on
    unsigned long m_millisecDelay; // Number of ms between light delays
    ToggleMode m_mode;


};




#endif
