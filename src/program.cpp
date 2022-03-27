
#include <iterator>

#include "program.h"

ProgramRef create(const DataAttribs& attribs);
{
    return ProgramRef(new Program(attribs));
}


DaemonRef create(const DataAttribs& attribs);
{
    return DaemonRef(new Daemon(attribs));
}
