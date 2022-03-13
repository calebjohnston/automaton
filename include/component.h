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
	int capacity = 0;
};

class Device {
public:
	Device(const DeviceAttribs& attribs)
		: name(attribs.name), devtype(attribs.type), power(attribs.power), frequency(attribs.freq) {}
	
	std::string name;
	Type devtype;
	float power;
	float frequency; // or datarate
};

class Component : public Device {
public:
	Component(const DeviceAttribs& attribs)
		: Device(attribs), buffersize(attribs.size), capacity(attribs.capacity) {}

	int buffersize;
	int capacity;
};
