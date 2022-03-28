
#include <iterator>

#include "program.h"

ProgramRef Program::create(const DataAttribs& attribs)
{
    return ProgramRef(new Program(attribs));
}

DaemonRef Daemon::create(const DataAttribs& attribs)
{
    return DaemonRef(new Daemon(attribs));
}
