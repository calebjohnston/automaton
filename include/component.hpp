//
//  component.hpp
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#include <string>

class Component {
public:
	enum class System {
		None,
		Processing,
		Electrical
	};
	enum class Type {
		None,
		Disk,
		Memory,
		Processor,
		Network,
		Battery,
		Power
	};
	
public:
	std::string name;
	System system;
	Type devtype;
	float power;
	float datarate;
	float buffersize;
	int price;
	int capacity; // does this work?
	
	Component() : name(""), system(System::None), devtype(Type::None), power(0), datarate(0),
				  buffersize(0), price(0), capacity(0) {};
	~Component() = default;
	
	bool operator ==(const Component& rhs) const {
		return  name == rhs.name && system == rhs.system &&
				devtype == rhs.devtype && power == rhs.power &&
				datarate == rhs.datarate && buffersize == rhs.buffersize &&
				price == rhs.price && capacity == rhs.capacity;
	}
	
	bool operator !=(const Component& rhs) const { return !(*this == rhs); }
	bool operator <(const Component& rhs) const { return name < rhs.name; }
	bool operator >(const Component& rhs) const { return name > rhs.name; }
	
	operator bool() const {
		return !name.empty() || (System::None != system && Type::None != devtype);
	}
};
