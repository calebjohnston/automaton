
#include <iterator>

#include "data.h"

DataRef Data::create(const DataAttribs& attribs)
{
    return DataRef(new Data(attribs));
}

FileRef File::create(const DataAttribs& attribs)
{
    return FileRef(new File(attribs));
}
