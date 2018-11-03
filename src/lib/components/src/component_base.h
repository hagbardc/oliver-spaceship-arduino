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
    *
    * @param json:  A JsonBuffer which can be used for emission of a state change to Serial
    */
    virtual void step(JsonObject &json) = 0;

    /*
     * Populates the json object with information for a state change
     * Assumes that createObject() had already been called on json
     *
     * @param json:  ArduinoJson json which will be populated with the change info
     * @param element:  Name of the changing element
     * @param action:  Name of the event that occurred
     * @param value:  Any value associated with that event
    */
    void populateStateChange(
        JsonObject &json, const char* element,
        const char *action, const char *value);

protected:
    bool m_recentStateChange;
    char m_componentName[20];  // Name of the component
};


#endif
