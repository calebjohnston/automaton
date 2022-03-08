//
//  actor.hpp
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#include <string>

#include "component.hpp"
#include "data.hpp"

// create normal OOP type hierarchy
	// should base types with pure virtual members be allowed??
// create comparators and null evaluator for each hierarchical base type
// create class factory constructors for each type hierarchy
// create variant collections for each hierarchical base type
// manage variant collections with composable functions
// create networks of variant collections using composable functions + shared_ptrs
// query networks of variant collections using composable functions

class Host {
public:
	std::string name;
	std::string description;
	Computer machine;
	Kernel system;
	
	bool operator ==(const Host& rhs) const {
		return  name == rhs.name && description == rhs.description;
	}
	
	bool operator !=(const Host& rhs) const { return !(*this == rhs); }
	bool operator <(const Host& rhs) const { return name < rhs.name; }
	bool operator >(const Host& rhs) const { return name > rhs.name; }
	
	operator bool() const { return !name.empty(); }
	
protected:
	Host() : name(""), description("") {};
	virtual ~Host() = default;
};

/*
class Software : public Data {
public:
	enum class Type {
		None,
		Ping,
		Inference,
		Reflection,
		Symplex,
		Autoregression
	};
	
public:
	Type packet;
	int version;
	
	bool operator ==(const Software& rhs) const {
		return  name == rhs.name && description == rhs.description &&
				packet == rhs.packet && size == rhs.size &&
				version == rhs.version && access == rhs.access;
	}
	
	bool operator !=(const Software& rhs) const { return !(*this == rhs); }
	bool operator <(const Software& rhs) const { return name < rhs.name; }
	bool operator >(const Software& rhs) const { return name > rhs.name; }
	
	operator bool() const {
		return !name.empty() || Type::None != packet;
	}
	
protected:
	Software() : Data(), packet(Type::None), version(0) {};
	virtual ~Software() = default;
};
*/
