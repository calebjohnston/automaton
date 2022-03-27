//
//  component.h
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <array>
#include <map>
#include <string>
#include <stdlib.h>

#include "node.h"

typedef std::shared_ptr<class Device> DeviceRef;
typedef std::shared_ptr<class Component> ComponentRef;

enum class Type {
	Disk,
	Memory,
	Processor,
	Network,
	Battery,
	Power,
	None
};

struct DeviceAttribs {
	std::string name = "";
	Type type = Type::None;
	float power = 0;
	float freq = 0;
	int size = 0;
};

class Device : public GridNode {
public:
	static DeviceRef create(const DeviceAttribs& attribs);

	virtual int add(GridNodeRef child) override { return -1; }

	virtual bool remove(size_t index) override { return false; }

	Type type() const { return _type; }
	float freq() const { return _frequency; }

protected:
	Device(const DeviceAttribs& attribs)
		: GridNode(attribs.power, {}, attribs.name), _type(attribs.type), _frequency(attribs.freq) {}

	Type _type;
	float _frequency; // or datarate
};

class Component : public Device {
public:
	static ComponentRef create(const DeviceAttribs& attribs);

	int size() const { return _buffersize; }

protected:
	Component(const DeviceAttribs& attribs)
		: Device(attribs), _buffersize(attribs.size) {}

	int _buffersize;
};
