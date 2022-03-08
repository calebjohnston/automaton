//
//  component.hpp
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

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

class Device {
public:
	std::string name;
	Type devtype;
	float power;
	float frequency; // or datarate
	int price;
	
	Device() : name(""), devtype(Type::None), power(0), frequency(0), price(0) {};
	~Device() = default;
	
	bool operator ==(const Device& rhs) const {
		return  name == rhs.name && devtype == rhs.devtype && power == rhs.power &&
				frequency == rhs.frequency && price == rhs.price;
	}
	
	bool operator !=(const Device& rhs) const { return !(*this == rhs); }
	bool operator <(const Device& rhs) const { return name < rhs.name; }
	bool operator >(const Device& rhs) const { return name > rhs.name; }
	
	operator bool() const {
		return !name.empty() || Type::None != devtype;
	}
};

class Component : public Device {
public:
	float buffersize;
	int capacity; // does this work?
	
	Component() : Device(), buffersize(0), capacity(0) {};
	~Component() = default;
};

class Computer {
public:
	Component disk;
	Component memory;
	Component processor;
	Component network;
	Device battery;
	Device power;
};
