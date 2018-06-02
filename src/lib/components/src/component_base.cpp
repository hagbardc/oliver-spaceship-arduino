#include <component_base.h>


void ComponentBase::populateStateChange(
    JsonObject &json, const char *element,
    const char *action, const char *value)
{
    if( !action || !value || !element) {
        return;
    }
    json["component"] = this->m_componentName;
    json["element"] = element;
    json["action"] = action;
    json["value"] = value;
}
