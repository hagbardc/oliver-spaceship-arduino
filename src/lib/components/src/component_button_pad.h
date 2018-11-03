#include <component_base.h>

#include <Arduino.h>
#include <ArduinoJson.h>



class ComponentButtonPad : public ComponentBase
{
public:

    // First three arguments specify the first pin of a 4 pin set
    // (So if 33 is passed for buttonSelectPinStart, m_btnSelectPins = {33, 34, 35, 36})
    ComponentButtonPad( uint8_t buttonSelectPinStart,
                        uint8_t buttonReadPinStart,
                        uint8_t ledSelectPinStart,
                        uint8_t colorPins[4][3]);

    ~ComponentButtonPad() {};

    virtual int getStateChange(JsonObject &jsonState) { return 1;}

    virtual void getCurrentState(JsonObject &jsonState) {} ;

    // The ->step functionality for the keyboard matrix is an interesting
    // one.  We need to decide if this will be implemented as a blocking step
    // or something that respects the tick...
    virtual void step(JsonObject &json) { this->scan(json); }


private:

    enum {
        NUM_LED_COLUMNS=(4),
        NUM_LED_ROWS=(4),
        NUM_BTN_COLUMNS=(4),
        NUM_BTN_ROWS=(4),
        NUM_COLORS=(3),
        MAX_DEBOUNCE=(3)
    };

    enum COLOR_MAPPING {
        LED_OFF = 0,
        LED_RED = 1,
        LED_GREEN = 2,
        LED_BLUE = 3
    };


    //  Sets up the pin default states, modes, etc
    void setupPins();

    void scan(JsonObject &json);

    uint8_t m_ledOutputs[ComponentButtonPad::NUM_LED_COLUMNS][ComponentButtonPad::NUM_LED_ROWS];
    unsigned long m_nextScan;

    uint8_t m_btnSelectPins[4];//   = {50,51,52,53};
    uint8_t m_btnReadPins[4];// = {46,47,48,49};
    uint8_t m_ledSelectPins[4];//   = {42,43,44,45};

    // RGB pins for each of 4 rows
    uint8_t m_colorPins[4][3];// =  {{22,24,26}, {30,31,32},{33,34,35},{36,37,38}};

    int8_t m_debounceCount[ComponentButtonPad::NUM_BTN_COLUMNS][ComponentButtonPad::NUM_BTN_ROWS];

    //  When we're doing our scanning, this keeps track of which column we're looking at
    uint8_t m_currentColumn;

};
