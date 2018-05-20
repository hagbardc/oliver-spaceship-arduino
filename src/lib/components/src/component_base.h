#ifndef INCLUDED_COMPONENT_BASE_H
#define INCLUDED_COMPONENT_BASE_H

#include <ArduinoJson.h>

class ComponentBase {
public:
    ComponentBase() { }
    virtual ~ComponentBase() {}

    /*
     * If the component has experienced a state change since the last time
     * the state was queried, populate the json object
     *
     * @param jsonState:  ArduinoJson json object that contains only information about the changed states
     * @return 0 if there was no state change since the last query, 1 if there was such a change
    */
    virtual int getStateChange(JsonObject &jsonState) = 0;

    /*
    * Populates the JSON object with the current state of the component, regardless
    * of whether there has been a state change or not
    */
    virtual void getCurrentState(JsonObject &jsonState) = 0;

    /*
    * Our 'tick' function - representing a cycle of the microcontroller
    */
    virtual void step() = 0;

protected:
    bool m_recentStateChange;
};


#endif
