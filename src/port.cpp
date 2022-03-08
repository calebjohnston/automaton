#include "port.h"


Port::Port()
{
    mAddress = "Port Washington";
}

void Port::setAddress(const std::string& addr)
{
    mAddress = addr;
}
