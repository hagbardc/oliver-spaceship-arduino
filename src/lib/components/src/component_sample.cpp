#include <component_sample.h>

ComponentSample::ComponentSample() : m_isPressed(0)
{
    this->m_recentStateChange = false;
}

ComponentSample::~ComponentSample() {

}

int ComponentSample::getStateChange(JsonObject &jsonState) {
    return 15;
}

void ComponentSample::getCurrentState(JsonObject &jsonState) {
}

void ComponentSample::step(JsonObject &json) {

}
