
#include "network.h"

using namespace std;

NetworkNodeRef NetworkNode::create(const NetworkAttribs& attribs, vector<GraphNodeRef> children)
{
    return NetworkNodeRef(new NetworkNode(std::move(children), attribs));
}
