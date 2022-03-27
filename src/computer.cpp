
#include <iterator>

#include "computer.h"

using namespace std;

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

	int idx;
	switch (component->type()) {
		case Type::Disk:
			idx = _diskIdx;
			_children[idx] = child;
			break;
		case Type::Memory:
			idx = _memoryIdx;
			_children[idx] = child;
			break;
		case Type::Processor:
			idx = _processorIdx;
			_children[idx] = child;
			break;
		case Type::Network:
			idx = _uplinkIdx;
			_children[idx] = child;
			break;
		case Type::Battery:
			idx = _batteryIdx;
			_children[idx] = child;
			break;
		case Type::Power:
			idx = _powerIdx;
			_children[idx] = child;
			break;
		default:
			idx = -1;
	}

	return idx;
}

bool Computer::remove(size_t index)
{
	if (index >= _maxChildren) return false;

	if (index < 4)
		_children[index] = ComponentRef(nullptr);
	else
		_children[index] = DeviceRef(nullptr);

	return true;
}
