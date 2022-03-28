
#include <iterator>

#include "computer.h"

using namespace std;

const map<Type,int> Computer::ComponentIndexMap = {
    { Type::Disk, Computer::DiskIdx },
    { Type::Memory, Computer::MemoryIdx },
    { Type::Processor, Computer::ProcessorIdx },
    { Type::Network, Computer::UplinkIdx },
    { Type::Battery, Computer::BatteryIdx },
    { Type::Power, Computer::PowerIdx }
};

ComputerRef Computer::create(const ComputerAttribs& attribs)
{
    return ComputerRef(new Computer(attribs));
}

Computer::Computer(const ComputerAttribs& attribs)
	: GridNode(attribs.power, {}, attribs.name), _serial(attribs.serial), _manufacturer(attribs.manufacturer)
{
	disk(ComponentRef(nullptr));
	memory(ComponentRef(nullptr));
	processor(ComponentRef(nullptr));
	uplink(ComponentRef(nullptr));
	battery(DeviceRef(nullptr));
	powerUnit(DeviceRef(nullptr));
}

int Computer::add(GridNodeRef child)
{
	DeviceRef component = dynamic_pointer_cast<Device>(child);
	if (!component) return -1;

    auto search = ComponentIndexMap.find(component->type());
    if (search == ComponentIndexMap.end()) return -1;

    int idx = search->second;
    _children[idx] = child;
    return idx;
}

bool Computer::remove(size_t index)
{
	if (index >= Computer::MaxChildren) return false;

	if (index < 4)
		_children[index] = ComponentRef(nullptr);
	else
		_children[index] = DeviceRef(nullptr);

	return true;
}
