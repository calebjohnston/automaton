//
//  program.hpp
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#include <string>

class Software {
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
	std::string name;
	std::string description;
	Type packet;
	int size;
	int version;
	std::string access; // revise...
	
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
	Software() : name(""), description(""), packet(Type::None), size(1), version(0), access("") {};
	virtual ~Software() = default;
};

class Program : public Software {
public:
	enum class Encryption {
		None,
		DH4,
		X509,
		Kerberos
	};
	
public:
	Encryption crypto;
	int cycles;
	
	Program() : Software(), crypto(Encryption::None), cycles(1) {};
	virtual ~Program() = default;
};

class Daemon : public Software {
public:
	float efficiency;
	
	Daemon() : Software(), efficiency(1) {};
	~Daemon() = default;
};
