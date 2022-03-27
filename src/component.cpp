
#include "component.h"

using namespace std;

DeviceRef Device::create(const DeviceAttribs& attribs)
{
    return DeviceRef(new Device(attribs));
}

ComponentRef Component::create(const DeviceAttribs& attribs)
{
    return ComponentRef(new Component(attribs));
}
