//
//  program.hpp
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

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
		return !name.empty() && size > 0;
	}
	
protected:
	Data() : name(""), description(""), size(1), access("") {};
	
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
	std::string command;	// used to match the CLI input
	
protected:
	Software() : Data(), packet(Type::None), version(0) {};
	
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
};

class Daemon : public Software {
public:
	float efficiency;
	
	Daemon() : Software(), efficiency(1) {};
};

