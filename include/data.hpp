//
//  program.hpp
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#include <string>
#include <vector>

class Data {
public:
	std::string name;
	std::string description;
	int size;
	std::string access; // revise...
	
	bool operator ==(const Data& rhs) const {
		return  name == rhs.name && description == rhs.description &&
		size == rhs.size && access == rhs.access;
	}
	
	bool operator !=(const Data& rhs) const { return !(*this == rhs); }
	bool operator <(const Data& rhs) const { return name < rhs.name; }
	bool operator >(const Data& rhs) const { return name > rhs.name; }
	
	operator bool() const {
		return !name.empty();
	}
	
protected:
	Data() : name(""), description(""), size(1), access("") {};
	virtual ~Data() = default;
};

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


class Kernel {
public:
	std::string name;
	std::string description;
	std::vector<Daemon> daemons;
	std::vector<Program> programs;
	std::vector<Data> filesystem;
};
