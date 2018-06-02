#ifndef INCLUDED_COMPONENT_SAMPLE_H
#define INCLUDED_COMPONENT_SAMPLE_H

#include <component_base.h>

#include <ArduinoJson.h>

class ComponentSample : public ComponentBase {
public:
    ComponentSample();
    ~ComponentSample();

    virtual int getStateChange(JsonObject &jsonState);

    virtual void getCurrentState(JsonObject &jsonState);

    virtual void step(JsonObject &json);

private:
    int m_isPressed;
};

#endif
